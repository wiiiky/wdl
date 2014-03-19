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
#ifndef __WL_BTER_H__
#define __WL_BTER_H__

#include <gtk/gtk.h>
#include "libtransmission/transmission.h"
#include "libtransmission/variant.h"

G_BEGIN_DECLS
/***
****
***/
#define MEM_K 1024
#define MEM_K_STR "KiB"
#define MEM_M_STR "MiB"
#define MEM_G_STR "GiB"
#define MEM_T_STR "TiB"
#define DISK_K 1000
#define DISK_B_STR   "B"
#define DISK_K_STR "kB"
#define DISK_M_STR "MB"
#define DISK_G_STR "GB"
#define DISK_T_STR "TB"
#define SPEED_K 1000
#define SPEED_B_STR  "B/s"
#define SPEED_K_STR "kB/s"
#define SPEED_M_STR "MB/s"
#define SPEED_G_STR "GB/s"
#define SPEED_T_STR "TB/s"
/*
 * Macro Type
 */
#define WL_TYPE_BTER	(wl_bter_get_type())
#define WL_BTER(obj) (G_TYPE_CHECK_INSTANCE_CAST(\
			(obj),WL_TYPE_BTER,WlBter))
#define WL_IS_BTER(obj) (G_TYPE_CHECK_INSTANCE_TYPE(\
			(obj),WL_TYPE_BTER))
#define WL_BTER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST(\
			(klass),WL_TYPE_BTER,WlBterClass))
#define WL_IS_BTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE(\
			(klass),WL_TYPE_BTER))
#define WL_BTER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_TYPE(\
			(obj),WL_TYPE_BTER,WlBterClass))
typedef struct _WlBter WlBter;
typedef struct _WlBterClass WlBterClass;

struct _WlBter {
	GtkEventBox parent;
	/* libtransmission的会话，由外部传递进来 */
	tr_session *session;
	tr_torrent *torrent;
};

struct _WlBterClass {
	GtkEventBoxClass parentClass;
};

GType wl_bter_get_type(void) G_GNUC_CONST;

/*************************************************
 * PUBLIC
 ***********************************************/
WlBter *wl_bter_new(tr_session * session);

G_END_DECLS						/* __WL_BTER_H__ */
#endif
