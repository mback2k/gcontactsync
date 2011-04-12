LINUX32_COMPILER = linux32 gcc
LINUX64_COMPILER = gcc
WIN32_COMPILER = i586-mingw32msvc-gcc
WIN32_WINDRES = i586-mingw32msvc-windres
WIN32_OBJCOPY = i586-mingw32msvc-objcopy

WIN32_DEV_DIR = /home/marc/dev/win32-dev
WIN32_PIDGIN_DIR = /home/marc/dev/pidgin-2.7.11

PIDGIN_DIR = /home/marc/dev/pidgin-2.7.11

LIBGCAL_DIR = /home/marc/dev/libgcal
LIBCURL_DIR = /home/marc/dev/curl

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
	-I$(LIBCURL_DIR)/include \
	-I$(LIBGCAL_DIR)/inc \
	-Iinc

LINUX_LIBS = \
	-lgcal

WIN32_CFLAGS = \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include/glib-2.0 \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/lib/glib-2.0/include \
	-I${WIN32_DEV_DIR}/gtk_2_0-2.14/include \
	-I${WIN32_PIDGIN_DIR}/libpurple/win32 \
	-I${WIN32_PIDGIN_DIR}/libpurple \
	-I${WIN32_DEV_DIR}/libxml2-2.7.4/include/libxml2 \
        -I$(LIBCURL_DIR)/include \
	-I$(LIBGCAL_DIR)/inc \
	-Iinc \
	-Wno-format

WIN32_LIBS = \
	-L${WIN32_DEV_DIR}/gtk_2_0-2.14/bin \
	-L${WIN32_PIDGIN_DIR}/libpurple \
	-L$(LIBGCAL_DIR)/bin \
	-llibglib-2.0-0 \
	-lpurple \
	-llibgcal-0

GCONTACTSYNC_SOURCES = \
	src/gcontactsync.c \
	src/gcontactsync_debug.c \
	src/gcontactsync_gcal.c \
	src/gcontactsync_sync.c

LONG_BIT = $(shell getconf LONG_BIT)

.PHONY:	all clean install

all:	bin/gcontactsync32.so bin/gcontactsync64.so bin/gcontactsync.dll

install:	bin/gcontactsync${LONG_BIT}.so
	cp bin/gcontactsync${LONG_BIT}.so /usr/local/lib/purple-2/gcontactsync.so

uninstall:
	rm -f /usr/local/lib/purple-2/gcontactsync.so

clean:
	rm -f bin/gcontactsync32.so bin/gcontactsync64.so bin/gcontactsync.dll bin/gcontactsync.res


bin/gcontactsync32.so:	${GCONTACTSYNC_SOURCES}
	${LINUX32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${LINUX_CFLAGS} ${LINUX_LIBS} -I. -g -m32 -O2 -pipe ${GCONTACTSYNC_SOURCES} -o $@ -shared -fPIC -DPIC

bin/gcontactsync64.so:	${GCONTACTSYNC_SOURCES}
	${LINUX64_COMPILER} ${LIBPURPLE_CFLAGS} -Wall ${LINUX_CFLAGS} ${LINUX_LIBS} -I. -g -m64 -O2 -pipe ${GCONTACTSYNC_SOURCES} -o $@ -shared -fPIC -DPIC

bin/gcontactsync.res:	src/gcontactsync.rc
	${WIN32_WINDRES} $< -O coff -o $@

bin/gcontactsync.dll:	${GCONTACTSYNC_SOURCES} bin/gcontactsync.res
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe ${GCONTACTSYNC_SOURCES} bin/gcontactsync.res -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}
	${WIN32_OBJCOPY} --only-keep-debug $@ $@.dbg
	${WIN32_OBJCOPY} --strip-debug $@
	${WIN32_OBJCOPY} --add-gnu-debuglink=$@.dbg $@

bin/gcontactsync-debug.dll:	${GCONTACTSYNC_SOURCES} bin/gcontactsync.res
	${WIN32_COMPILER} ${LIBPURPLE_CFLAGS} -Wall -I. -g -O2 -pipe ${GCONTACTSYNC_SOURCES} ${WIN32_SOURCES} bin/gcontactsync.res -o $@ -shared -mno-cygwin ${WIN32_CFLAGS} ${WIN32_LIBS}

