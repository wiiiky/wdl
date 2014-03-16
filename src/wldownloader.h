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
#ifndef __WL_DOWNLOADER_H__
#define __WL_DOWNLOADER_H__

#include "wlhttper.h"
#include "wlhttpermenu.h"

G_BEGIN_DECLS
/*
 * Type macros
 */
#define WL_TYPE_DOWNLOADER (wl_downloader_get_type())
#define WL_DOWNLOADER(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WL_TYPE_DOWNLOADER,WlDownloader))
#define WL_IS_DOWNLOADER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WL_TYPE_DOWNLOADER))
#define WL_DOWNLOADER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WL_TYPE_DOWNLOADER,WlDownloaderClass))
#define WL_IS_DOWNLOADER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WL_TYPE_DOWNLOADER))
#define WL_DOWNLOADER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_TYPE((obj),\
				WL_TYPE_DOWNLOADER,WlDownloaderClass))
typedef struct _WlDownloader WlDownloader;
typedef struct _WlDownloaderClass WlDownloaderClass;

struct _WlDownloader {
	GtkScrolledWindow parent;
	/* Private */
	GtkWidget *vBox;
	GList *list;
	gpointer *selected;
};

struct _WlDownloaderClass {
	GtkScrolledWindowClass parentClass;
};

GType wl_downloader_get_type(void) G_GNUC_CONST;
/*
 * public functions
 */
WlDownloader *wl_downloader_new(void);
/*
 * @description 添加一个HTTP下载任务
 * @param url HTTP下载地址
 * @param path 文件保存路径
 * @return 返回添加的WlHttper对象
 */
WlHttper *wl_downloader_append_httper(WlDownloader * dl, const gchar * url,
									  const gchar * path);
/*
 * @description 移除一个HTTP下载任务
 * @param httper 移除的目标
 */
void wl_downloader_remove_httper(WlDownloader * dl, WlHttper * httper);
/*
 * @description 开始选中的下载任务
 */
void wl_downloader_start_selected(WlDownloader * dl);
/*
 * @description 暂停选中的下载任务
 */
void wl_downloader_pause_selected(WlDownloader * dl);
/*
 * @description 继续选中的下载任务
 */
void wl_downloader_continue_selected(WlDownloader * dl);
/*
 * @description 获取选中任务的状态
 * @return 如果没有选中，返回0
 */
WlHttperStatus wl_downloader_get_selected_status(WlDownloader * dl);
/*
 * @description 删除选中的任务
 */
void wl_downloader_remove_selected(WlDownloader * dl);


G_END_DECLS
#endif							/* __WL_DOWNLOADER_H__ */
