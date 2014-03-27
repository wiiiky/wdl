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

#include "wlbter.h"

enum {
	WL_BTER_PROPERTY_SESSION = 1,
	WL_BTER_PROPERTY_TORRENT,
};

G_DEFINE_TYPE(WlBter, wl_bter, GTK_TYPE_EVENT_BOX);

static void wl_bter_getter(GObject * object, guint property_id,
						   GValue * value, GParamSpec * ps);
static void wl_bter_setter(GObject * object, guint property_id,
						   const GValue * value, GParamSpec * ps);

static void wl_bter_init(WlBter * bter)
{
	GtkWidget *hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_container_set_border_width(GTK_CONTAINER(hBox), 0);
	gtk_container_add(GTK_CONTAINER(bter), hBox);

	GtkIconSize iconSize = GTK_ICON_SIZE_DIALOG;
	GtkWidget *image = gtk_image_new_from_icon_name("html",
													iconSize);
	gtk_box_pack_start(GTK_BOX(hBox), image, FALSE, FALSE, 0);

	GtkWidget *vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
	gtk_container_set_border_width(GTK_CONTAINER(vBox), 8);
	gtk_box_pack_start(GTK_BOX(hBox), vBox, TRUE, TRUE, 0);

	GtkWidget *titleLabel = gtk_label_new("New BitTorrent Download");
	PangoAttrList *attrList = pango_attr_list_new();
	pango_attr_list_insert(attrList,
						   pango_attr_weight_new(PANGO_WEIGHT_BOLD));
	gtk_label_set_attributes(GTK_LABEL(titleLabel), attrList);
	gtk_widget_set_halign(titleLabel, GTK_ALIGN_START);
	gtk_label_set_ellipsize(GTK_LABEL(titleLabel), PANGO_ELLIPSIZE_END);
	gtk_label_set_single_line_mode(GTK_LABEL(titleLabel), TRUE);
	gtk_box_pack_start(GTK_BOX(vBox), titleLabel, TRUE, TRUE, 0);

	/* 进度条 */
	GtkWidget *progressBar = gtk_progress_bar_new();
	gtk_box_pack_start(GTK_BOX(vBox), progressBar, TRUE, TRUE, 0);

	/* 下载信息，速度等 */
	GtkWidget *iBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	gtk_box_pack_start(GTK_BOX(vBox), iBox, TRUE, TRUE, 0);

	GtkWidget *dlLabel = gtk_label_new(" 0 KB");
	gtk_widget_set_halign(dlLabel, GTK_ALIGN_START);
	gtk_label_set_single_line_mode(GTK_LABEL(dlLabel), TRUE);
	gtk_box_pack_start(GTK_BOX(iBox), dlLabel, FALSE, FALSE, 0);

	GtkWidget *totalLabel = gtk_label_new("of 0 KB -");
	gtk_label_set_single_line_mode(GTK_LABEL(totalLabel), TRUE);
	gtk_box_pack_start(GTK_BOX(iBox), totalLabel, FALSE, FALSE, 0);

	GtkWidget *arrow =
		gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_ETCHED_OUT);
	gtk_box_pack_start(GTK_BOX(iBox), arrow, FALSE, FALSE, 0);
	GtkWidget *speedLabel = gtk_label_new(" 0.00 KB/S");
	gtk_label_set_single_line_mode(GTK_LABEL(speedLabel), TRUE);
	gtk_box_pack_start(GTK_BOX(iBox), speedLabel, FALSE, FALSE, 0);

	GtkWidget *timeLabel = gtk_label_new(" (unknown time remaining)");
	gtk_label_set_single_line_mode(GTK_LABEL(timeLabel), TRUE);
	gtk_box_pack_start(GTK_BOX(iBox), timeLabel, FALSE, FALSE, 0);

	/* TODO */


	bter->session = NULL;
	bter->torrent = NULL;
}

static void wl_bter_class_init(WlBterClass * klass)
{
	GObjectClass *objClass = G_OBJECT_CLASS(klass);
	objClass->get_property = wl_bter_getter;
	objClass->set_property = wl_bter_setter;

	GParamSpec *ps;
	ps = g_param_spec_pointer("session",
							  "libtransmission session",
							  "Libtransmission Session",
							  G_PARAM_READABLE | G_PARAM_WRITABLE |
							  G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(objClass, WL_BTER_PROPERTY_SESSION,
									ps);

	ps = g_param_spec_pointer("torrent",
							  "libtransmission torrent",
							  "Libtransmission Torrent",
							  G_PARAM_READABLE | G_PARAM_WRITABLE |
							  G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(objClass, WL_BTER_PROPERTY_TORRENT,
									ps);
}

static void wl_bter_getter(GObject * object, guint property_id,
						   GValue * value, GParamSpec * ps)
{
	WlBter *bter = WL_BTER(object);
	switch (property_id) {
	case WL_BTER_PROPERTY_SESSION:
		g_value_set_pointer(value, bter->session);
		break;
	case WL_BTER_PROPERTY_TORRENT:
		g_value_set_pointer(value, bter->torrent);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, ps);
		break;
	}
}

static void wl_bter_setter(GObject * object, guint property_id,
						   const GValue * value, GParamSpec * ps)
{
	WlBter *bter = WL_BTER(object);
	switch (property_id) {
	case WL_BTER_PROPERTY_SESSION:
		bter->session = g_value_get_pointer(value);
		break;
	case WL_BTER_PROPERTY_TORRENT:
		bter->torrent = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, ps);
		break;
	}
}


/**********************************************************
 * PUBLIC
 *******************************************************/
WlBter *wl_bter_new(tr_session * session, tr_torrent * torrent)
{
	g_return_val_if_fail(session != NULL && torrent != NULL, NULL);
	WlBter *bter = (WlBter *) g_object_new(WL_TYPE_BTER,
										   "session", session,
										   "torrent", torrent,
										   NULL);
	return bter;
}

WlBter *wl_bter_new_from_file(tr_session * session, const gchar * path)
{
	g_return_val_if_fail(session != NULL && path != NULL, NULL);
	tr_torrent *torrent;
	tr_ctor *ctor = tr_ctorNew(session);
	if (ctor) {
		tr_ctorSetMetainfoFromFile(ctor, path);
		torrent = tr_torrentNew(ctor, NULL, NULL);
		tr_ctorFree(ctor);
	} else
		return NULL;
	/* 路径不对? */
	if (torrent == NULL)
		return NULL;
	WlBter *bter = (WlBter *) g_object_new(WL_TYPE_BTER,
										   "session", session,
										   "torrent", torrent,
										   NULL);
	return bter;
}

WlBter *wl_bter_new_from_magnetlink(tr_session * session,
									const gchar * link)
{
	g_return_val_if_fail(session != NULL && link != NULL, NULL);
	tr_torrent *torrent;
	tr_ctor *ctor = tr_ctorNew(session);
	if (ctor) {
		tr_ctorSetMetainfoFromMagnetLink(ctor, link);
		torrent = tr_torrentNew(ctor, NULL, NULL);
		tr_ctorFree(ctor);
	} else
		return NULL;
	/* MagnetLink 无效? */
	if (torrent == NULL)
		return NULL;
	WlBter *bter = (WlBter *) g_object_new(WL_TYPE_BTER,
										   "session", session,
										   "torrent", torrent,
										   NULL);
	return bter;
}


void wl_bter_start(WlBter * bter)
{
	g_return_if_fail(WL_IS_BTER(bter) && bter->torrent != NULL);
	tr_torrentStart(bter->torrent);
}
