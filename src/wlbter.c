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

G_DEFINE_TYPE(WlBter,wl_bter,GTK_TYPE_EVENT_BOX);

static void wl_bter_getter(GObject *object,guint property_id,
								GValue *value,GParamSpec *ps);
static void wl_bter_setter(GObject *object,guint property_id,
								const GValue *value,GParamSpec *ps);

static void wl_bter_init(WlBter *bter)
{
	GtkWidget *hBox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,6);
	gtk_container_set_border_width(GTK_CONTAINER(hBox),0);
	gtk_container_add(GTK_CONTAINER(bter),hBox);

	GtkIconSize iconSize=GTK_ICON_SIZE_DIALOG;
	GtkWidget *image=gtk_image_new_from_icon_name("html",
				iconSize);
	gtk_box_pack_start(GTK_BOX(hBox),image,FALSE,FALSE,0);

	GtkWidget *vBox=gtk_box_new(GTK_ORIENTATION_VERTICAL,3);
	gtk_container_set_border_width(GTK_CONTAINER(vBox),8);
	gtk_box_pack_start(GTK_BOX(hBox),vBox,TRUE,TRUE,0);

	GtkWidget *titleLabel=gtk_label_new("New BitTorrent Download");
	PangoAttrList *attrList=pango_attr_list_new();
	pango_attr_list_insert(attrList,
				pango_attr_weight_new(PANGO_WEIGHT_BOLD));
	gtk_label_set_attributes(GTK_LABEL(titleLabel),attrList);
	gtk_widget_set_halign(titleLabel,GTK_ALIGN_START);
	gtk_label_set_ellipsize(GTK_LABEL(titleLabel),PANGO_ELLIPSIZE_END);
	gtk_label_set_single_line_mode(GTK_LABEL(titleLabel),TRUE);
	gtk_box_pack_start(GTK_BOX(vBox),titleLabel,TRUE,TRUE,0);



	bter->session=NULL;
	bter->torrent=NULL;
}

static void wl_bter_class_init(WlBterClass *klass)
{
	GObjectClass *objClass=G_OBJECT_CLASS(klass);
	objClass->get_property=wl_bter_getter;
	objClass->set_property=wl_bter_setter;
}

static void wl_bter_getter(GObject *object,guint property_id,
								GValue *value,GParamSpec *ps)
{
	WlBter *bter=WL_BTER(object);
	switch(property_id){
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object,property_id,ps);
			break;
	}
}
static void wl_bter_setter(GObject *object,guint property_id,
								const GValue *value,GParamSpec *ps)
{
	WlBter *bter=WL_BTER(object);
	switch(property_id){
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object,property_id,ps);
			break;
	}
}

