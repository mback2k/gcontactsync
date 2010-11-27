LINUX32_COMPILER = linux32 gcc
LINUX64_COMPILER = gcc
WIN32_COMPILER = i586-mingw32msvc-gcc
WIN32_WINDRES = i586-mingw32msvc-windres
WIN32_OBJCOPY = i586-mingw32msvc-objcopy

WIN32_DEV_DIR = /home/marc/Dokumente/win32-dev
WIN32_PIDGIN_DIR = /home/marc/Dokumente/pidgin-2.7.7

PIDGIN_DIR = /home/marc/Dokumente/pidgin-2.7.7

LIBGCAL_DIR = libgcal-0.9.6
LIBCURL_DIR = curl-7.21.2
ZLIB_DIR = zlib125-dll

LIBPURPLE_CFLAGS = \
	-DPURPLE_PLUGINS \
	-DENABLE_NLS \

LINUX_CFLAGS = \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include \
	-I/usr/local/include/glib-2.0 \
	-I/usr/local/lib/glib-2.0/include \
	-I/usr/local/include \
	-I/usr/include/libpurple \
	-I/usr/local/include/libpurple \
	-I/usr/include/libxml2 \
	-I/usr/local/include/libxml2 \
	-I/usr/include/libgcal \
	-I/usr/local/include/libgcal \
	-I$(LIBCURL_DIR)/include

LINUX_LIBS = \
	-lcurl \
	-lgcal

WIN32_CFLAGS = \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include/glib-2.0 \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/lib/glib-2.0/include \
	-I${WIN32_PIDGIN_DIR}/libpurple/win32 \
	-I${WIN32_PIDGIN_DIR}/libpurple \
	-I${WIN32_DEV_DIR}/libxml2-2.7.4/include/libxml2 \
	-I$(LIBCURL_DIR)/include \
	-I$(LIBGCAL_DIR)/inc \
	-Wno-format

WIN32_LIBS = \
	-L${WIN32_DEV_DIR}/gtk_2_0-2.14/bin \
	-L${WIN32_PIDGIN_DIR}/libpurple \
	-L$(ZLIB_DIR) \
	-Llib \
	-L. \
	-llibglib-2.0-0 \
	-lpurple \
	-llibcurl \
	-llibgcal

WIN32_LIBGCAL_CFLAGS = \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include/glib-2.0 \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/lib/glib-2.0/include \
	-I${WIN32_DEV_DIR}/libxml2-2.7.4/include/libxml2 \
	-I$(LIBCURL_DIR)/include \
	-I$(LIBGCAL_DIR)/inc \
	-Wno-pointer-sign

WIN32_LIBGCAL_LIBS = \
	-L${WIN32_DEV_DIR}/gtk_2_0-2.14/bin \
	-L${WIN32_DEV_DIR}/libxml2-2.7.4/bin \
	-L$(ZLIB_DIR) \
	-Llib \
	-llibglib-2.0-0 \
	-lxml2-2 \
	-llibcurl

WIN32_LIBGCAL_SOURCES = \
	$(LIBGCAL_DIR)/src/atom_parser.c \
	$(LIBGCAL_DIR)/src/gcal.c \
	$(LIBGCAL_DIR)/src/gcalendar.c \
	$(LIBGCAL_DIR)/src/gcal_parser.c \
	$(LIBGCAL_DIR)/src/gcal_status.c \
	$(LIBGCAL_DIR)/src/gcontact.c \
	$(LIBGCAL_DIR)/src/gcont.c \
	$(LIBGCAL_DIR)/src/xml_aux.c

GCONTACTSYNC_SOURCES = \
	gcontactsync.c



.PHONY:	all clean install

all:	gcontactsync32.so gcontactsync64.so gcontactsync.dll libgcal.dll

install:
	test   -d /usr/lib64 || cp gcontactsync32.so /usr/local/lib/purple-2/gcontactsync.so
	test ! -d /usr/lib64 || cp gcontactsync64.so /usr/local/lib/purple-2/gcontactsync.so

uninstall:
	rm -f /usr/local/lib/purple-2/gcontactsync.so

clean:
	rm -f gcontactsync32.so gcontactsync64.so gcontactsync.dll libgcal.dll


gcontactsync32.so:	${GCONTACTSYNC_SOURCES}
	${LINUX32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${LINUX_CFLAGS} ${LINUX_LIBS} -I. -g -m32 -O2 -pipe ${GCONTACTSYNC_SOURCES} -o $@ -shared -fPIC -DPIC

gcontactsync64.so:	${GCONTACTSYNC_SOURCES}
	${LINUX64_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${LINUX_CFLAGS} ${LINUX_LIBS} -I. -g -m64 -O2 -pipe ${GCONTACTSYNC_SOURCES} -o $@ -shared -fPIC -DPIC

gcontactsync.res:	gcontactsync.rc
	${WIN32_WINDRES} $< -O coff -o $@

gcontactsync.dll:	${GCONTACTSYNC_SOURCES} gcontactsync.res libgcal.dll
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe ${GCONTACTSYNC_SOURCES} gcontactsync.res -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}
	${WIN32_OBJCOPY} --only-keep-debug $@ $@.dbg
	${WIN32_OBJCOPY} --strip-debug $@
	${WIN32_OBJCOPY} --add-gnu-debuglink=$@.dbg $@

gcontactsync-debug.dll:	${GCONTACTSYNC_SOURCES} gcontactsync.res
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe ${GCONTACTSYNC_SOURCES} ${WIN32_SOURCES} gcontactsync.res -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}

libgcal.res:		libgcal.rc
	${WIN32_WINDRES} $< -O coff -o $@

libgcal.dll:		$(WIN32_LIBGCAL_SOURCES) libgcal.res
	${WIN32_COMPILER} -Wall -I. -g -O2 -pipe ${WIN32_LIBGCAL_SOURCES} libgcal.res -o $@ -shared -mno-cygwin ${WIN32_LIBGCAL_CFLAGS} ${WIN32_LIBGCAL_LIBS}
	${WIN32_OBJCOPY} --only-keep-debug $@ $@.dbg
	${WIN32_OBJCOPY} --strip-debug $@
	${WIN32_OBJCOPY} --add-gnu-debuglink=$@.dbg $@

libgcal-debug.dll:	$(WIN32_LIBGCAL_SOURCES) libgcal.res
	${WIN32_COMPILER} -Wall -I. -g -O2 -pipe ${WIN32_LIBGCAL_SOURCES} libgcal.res -o $@ -shared -mno-cygwin ${WIN32_LIBGCAL_CFLAGS} ${WIN32_LIBGCAL_LIBS}

