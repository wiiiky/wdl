## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
			  -DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
			  -DPACKAGE_SRC_DIR=\""$(srcdir). \
			  $(libcurl_CFLAGS). \
			  $(gtk+-3.0_CFLAGS)"\" \
			  -DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
			  $(WDL_CFLAGS)

AM_CFLAGS =\
		   -Wall\
		   -g

bin_PROGRAMS = wdl

wdl_SOURCES =  \
			   main.c \
			   wldownloader.c \
			   wldownloader.h \
			   wldownloadwindow.c \
			   wldownloadwindow.h \
			   wlhttper.c \
			   wlhttper.h \
			   wlhttpermenu.c \
			   wlhttpermenu.h \
			   wlhttperproperties.c \
			   wlhttperproperties.h \
			   wlurldialog.c \
			   wlurldialog.h \
			   wlbter.c \
			   wlbter.h \
			   wlbtfilechooser.c \
			   wlbtfilechooser.h \
			   icons.c \
			   icons.h \
			   wlbtermenu.c \
			   wlbtermenu.h \
			   wlbterproperties.c \
			   wlbterproperties.h

wdl_LDFLAGS = \
			  -Wl,--export-dynamic 

wdl_CFLAGS =-pthread

wdl_LDADD = \
			$(srcdir)/libtransmission.a	\
			$(srcdir)/libdht.a \
			$(srcdir)/libminiupnp.a \
			$(srcdir)/libnatpmp.a \
			$(srcdir)/libutp.a \
			$(WDL_LIBS) \
			$(gtk+-3.0_LIBS) \
			$(libcurl_LIBS)	

SUBDIRS = \
	libtransmission
