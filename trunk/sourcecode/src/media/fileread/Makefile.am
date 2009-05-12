## Source directory

h_sources = $(top_srcdir)/include/media/fileread.h
cc_sources = fileread_api.c  fileread_cb.c  fileread_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libfileread.la
libfileread_la_SOURCES= $(h_sources) $(cc_sources)
#libfileread_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

