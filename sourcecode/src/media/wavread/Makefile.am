## Source directory

h_sources = $(top_srcdir)/include/media/wavread.h
cc_sources = wavread_api.c  wavread_cb.c  wavread_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libwavread.la
libwavread_la_SOURCES= $(h_sources) $(cc_sources)
#libwavread_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

