if RC
.rc.lo:
	$(AM_V_GEN)$(LIBTOOL) --tag=RC --mode=compile $(RC) -J rc -O coff -i $< -o $@
endif

####################################################################################################

#ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = gcontactsync.la

include_HEADERS = inc/gcontactsync.h inc/gcontactsync_debug.h inc/gcontactsync_gcal.h inc/gcontactsync_sync.h inc/gcontactsync_gui.h

gcontactsync_la_SOURCES = src/gcontactsync.c src/gcontactsync_debug.c src/gcontactsync_gcal.c src/gcontactsync_sync.c src/gcontactsync_gui.c

if RC
gcontactsync_la_SOURCES += src/gcontactsync_res.rc
endif

gcontactsync_la_CPPFLAGS = -Iinc
gcontactsync_la_CFLAGS = $(AM_CFLAGS) $(GLIB_CFLAGS) $(GTK_CFLAGS) $(PURPLE_CFLAGS) $(LIBXML_CFLAGS) $(LIBCURL_CFLAGS) $(LIBGCAL_CFLAGS)
gcontactsync_la_LIBADD = $(GLIB_LIBS) $(GTK_LIBS) $(PURPLE_LIBS) $(LIBXML_LIBS) $(LIBCURL_LIBS) $(LIBGCAL_LIBS)
gcontactsync_la_LDFLAGS = -module -avoid-version

dist_noinst_DATA = src/gcontactsync_res.rc

