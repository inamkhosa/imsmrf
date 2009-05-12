## Source directory

h_sources = $(top_srcdir)/include/media/wavwrite.h
cc_sources = wavwrite_api.c  wavwrite_cb.c  wavwrite_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libwavwrite.la
libwavwrite_la_SOURCES= $(h_sources) $(cc_sources)
#libwavwrite_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

