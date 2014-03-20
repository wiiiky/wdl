/* 
 * Copyright (C) 2014-2014 Wiky L(wiiiky@yeah.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "wldownloader.h"

enum {
	WL_DOWNLOADER_PROPERTY_SPACING = 1,
};

G_DEFINE_TYPE(WlDownloader, wl_downloader, GTK_TYPE_SCROLLED_WINDOW);

static void wl_downloader_get_property(GObject * object, guint property_id,
									   GValue * value, GParamSpec * ps);
static void wl_downloader_set_property(GObject * object, guint property_id,
									   const GValue * value,
									   GParamSpec * ps);
static gpointer wl_downloader_httper_pressed_callback(GtkWidget * widget,
													  GdkEventButton *
													  event,
													  gpointer data);
static inline void wl_downloader_set_httper_selected(WlDownloader * dl,
													 WlHttper * httper);

static void wl_downloader_init(WlDownloader * dl)
{
	g_object_set(G_OBJECT(dl), "hadjustment", NULL,
				 "vadjustment", NULL, NULL);

	GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(dl), vBox);

	dl->vBox = vBox;
	dl->list = NULL;
	dl->selected = NULL;
	dl->httperSelected = NULL;
	dl->httperSelectedData = NULL;
	dl->httperStatus = NULL;
	dl->httperStatusData = NULL;
}

static void wl_downloader_finalize(GObject * object)
{
	WlDownloader *dl = WL_DOWNLOADER(object);
	if (dl->list)
		g_list_free(dl->list);
}

static void wl_downloader_class_init(WlDownloaderClass * klass)
{
	GObjectClass *objClass = G_OBJECT_CLASS(klass);
	objClass->get_property = wl_downloader_get_property;
	objClass->set_property = wl_downloader_set_property;
	objClass->finalize = wl_downloader_finalize;

	GParamSpec *ps;
	ps = g_param_spec_uint("spacing",
						   "box spacing",
						   "Box Spacing",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(objClass,
									WL_DOWNLOADER_PROPERTY_SPACING, ps);
}

static void wl_downloader_get_property(GObject * object, guint property_id,
									   GValue * value, GParamSpec * ps)
{
	WlDownloader *dl = WL_DOWNLOADER(object);
	switch (property_id) {
	case WL_DOWNLOADER_PROPERTY_SPACING:
		g_value_set_uint(value, gtk_box_get_spacing(GTK_BOX(dl->vBox)));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, ps);
		break;
	}
}

static void wl_downloader_set_property(GObject * object, guint property_id,
									   const GValue * value,
									   GParamSpec * ps)
{
	WlDownloader *dl = WL_DOWNLOADER(object);
	switch (property_id) {
	case WL_DOWNLOADER_PROPERTY_SPACING:
		gtk_box_set_spacing(GTK_BOX(dl->vBox), g_value_get_uint(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, ps);
		break;
	}
}

static inline void wl_downloader_set_httper_selected(WlDownloader * dl,
													 WlHttper * httper)
{
	if (dl->selected == (gpointer) httper)
		return;
	if (dl->selected) {
		if (WL_IS_HTTPER(dl->selected)) {
			wl_httper_clear_highlight(WL_HTTPER(dl->selected));
			wl_httper_set_status_callback(WL_HTTPER(dl->selected), NULL,
										  NULL);
		}
	}
	dl->selected = NULL;
	if (httper == NULL) {
		goto CALLBACK;
	} else if (WL_IS_HTTPER(httper)) {
		wl_httper_highlight(httper);
		wl_httper_set_status_callback(httper, dl->httperStatus,
									  dl->httperStatusData);
	} else
		return;					/* unknown type */

	dl->selected = (gpointer) httper;
  CALLBACK:
	if (dl->httperSelected)
		dl->httperSelected(dl, dl->httperSelectedData);
}

static void on_remove_httper_activate(GtkMenuItem * item, gpointer data)
{
	WlHttper *httper = (WlHttper *) data;
	WlDownloader *dl = wl_httper_get_user_data(httper);
	wl_downloader_remove_httper(dl, httper);
}

static void on_delete_files_activate(GtkMenuItem * item, gpointer data)
{
	WlHttper *httper = (WlHttper *) data;
	WlDownloader *dl = wl_httper_get_user_data(httper);

	const gchar *path = wl_httper_get_path(httper);
	if (path) {
		GFile *file = g_file_new_for_path(path);
		/* 将文件放入回收站或者彻底删除 */
		if (g_file_trash(file, NULL, NULL) == FALSE) {
			g_file_delete(file, NULL, NULL);
		}
		g_object_unref(file);
	}
	wl_downloader_remove_httper(dl, httper);
}

static inline GtkWidget *wl_downloader_httper_popmenu(WlHttper * httper)
{
	GtkWidget *menu = wl_httper_get_popmenu(httper);
	if (menu == NULL) {
		menu = wl_httper_menu_new(httper);
		wl_httper_menu_append_separator(WL_HTTPER_MENU(menu));

		GtkWidget *rmHttper =
			gtk_image_menu_item_new_from_stock(GTK_STOCK_REMOVE, NULL);
		gtk_menu_item_set_label(GTK_MENU_ITEM(rmHttper), "Remove");
		gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM
												  (rmHttper), TRUE);
		g_signal_connect(G_OBJECT(rmHttper), "activate",
						 G_CALLBACK(on_remove_httper_activate), httper);
		wl_httper_menu_append(WL_HTTPER_MENU(menu), rmHttper);

		GtkWidget *dFile =
			gtk_image_menu_item_new_from_stock(GTK_STOCK_DELETE, NULL);
		gtk_menu_item_set_label(GTK_MENU_ITEM(dFile),
								"Remove and Delete Files");
		gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM
												  (dFile), TRUE);
		g_signal_connect(G_OBJECT(dFile), "activate",
						 G_CALLBACK(on_delete_files_activate), httper);
		wl_httper_menu_append(WL_HTTPER_MENU(menu), dFile);
	}
	return menu;
}

static gpointer wl_downloader_httper_pressed_callback(GtkWidget * widget,
													  GdkEventButton *
													  event, gpointer data)
{
	WlHttper *httper = WL_HTTPER(widget);
	WlDownloader *dl = (WlDownloader *) data;
	GtkWidget *popmenu;
	if (event->type == GDK_BUTTON_PRESS) {
		/* 单击 */
		wl_downloader_set_httper_selected(dl, httper);
		if (event->button == 1) {
			/* 左键 */
			/*wl_downloader_set_httper_selected(dl, httper); */
		} else if (event->button == 3) {
			popmenu = wl_downloader_httper_popmenu(httper);
			gtk_menu_popup(GTK_MENU(popmenu), NULL, NULL, NULL, NULL,
						   event->button,
						   gdk_event_get_time((GdkEvent *) event));
			/* 右键 */
		}
	} else if (event->type == GDK_2BUTTON_PRESS) {
		/* 双击 */
		if (event->button == 1) {
			/* 左键 */
		} else if (event->button == 3) {
			/* 右键 */
		}
	}
	return FALSE;
}

/**************************************************
 * PUBLIC
 ***************************************************/
WlDownloader *wl_downloader_new(void)
{
	WlDownloader *dl =
		(WlDownloader *) g_object_new(WL_TYPE_DOWNLOADER, NULL);
	return dl;
}

WlHttper *wl_downloader_append_httper(WlDownloader * dl, const gchar * url,
									  const gchar * path)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	WlHttper *httper = wl_httper_new(url, path);
	wl_httper_set_user_data(httper, dl);
	g_signal_connect(G_OBJECT(httper), "button-press-event",
					 G_CALLBACK(wl_downloader_httper_pressed_callback),
					 dl);

	gtk_box_pack_start(GTK_BOX(dl->vBox), GTK_WIDGET(httper), FALSE, FALSE,
					   0);
	dl->list = g_list_append(dl->list, httper);
	gtk_widget_show_all(GTK_WIDGET(httper));
	return httper;
}

void wl_downloader_remove_httper(WlDownloader * dl, WlHttper * httper)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl) && WL_IS_HTTPER(httper));
	dl->list = g_list_remove(dl->list, httper);
	wl_httper_abort(httper);
	gtk_container_remove(GTK_CONTAINER(dl->vBox), GTK_WIDGET(httper));
	wl_downloader_set_httper_selected(dl, NULL);
}

void wl_downloader_start_selected(WlDownloader * dl)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	if (dl->selected == NULL)
		return;
	if (WL_IS_HTTPER(dl->selected)) {
		WlHttper *httper = WL_HTTPER(dl->selected);
		wl_httper_start(httper);
	}
}

void wl_downloader_pause_selected(WlDownloader * dl)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	if (dl->selected == NULL)
		return;
	if (WL_IS_HTTPER(dl->selected)) {
		WlHttper *httper = WL_HTTPER(dl->selected);
		wl_httper_pause(httper);
	}
}

void wl_downloader_continue_selected(WlDownloader * dl)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	if (dl->selected == NULL)
		return;
	if (WL_IS_HTTPER(dl->selected)) {
		WlHttper *httper = WL_HTTPER(dl->selected);
		wl_httper_continue(httper);
	}
}

WlHttperStatus wl_downloader_get_selected_status(WlDownloader * dl)
{
	g_return_val_if_fail(WL_IS_DOWNLOADER(dl), 0);
	if (dl->selected == NULL)
		return 0;
	if (WL_IS_HTTPER(dl->selected))
		return wl_httper_get_status(WL_HTTPER(dl->selected));
	return 0;
}

gpointer wl_downloader_get_selected(WlDownloader * dl)
{
	g_return_val_if_fail(WL_IS_DOWNLOADER(dl), NULL);
	return dl->selected;
}

void wl_downloader_remove_selected(WlDownloader * dl)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	if (dl->selected == NULL)
		return;
	if (WL_IS_HTTPER(dl->selected)) {
		wl_downloader_remove_httper(dl, WL_HTTPER(dl->selected));
	}
}

void wl_downloader_set_selected_callback(WlDownloader * dl,
										 WlHttperSelectedCallback callback,
										 gpointer data)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	dl->httperSelected = callback;
	dl->httperSelectedData = data;
}

void wl_downloader_set_httper_status_callback(WlDownloader * dl,
											  WlHttperStatusCallback
											  callback, gpointer data)
{
	g_return_if_fail(WL_IS_DOWNLOADER(dl));
	dl->httperStatus = callback;
	dl->httperStatusData = data;
}
