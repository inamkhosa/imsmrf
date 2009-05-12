## Source directory

h_sources = $(top_srcdir)/include/media/gsmenc.h
cc_sources = gsmenc_api.c  gsmenc_cb.c  gsmenc_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libgsmenc.la
libgsmenc_la_SOURCES= $(h_sources) $(cc_sources)
#libgsmenc_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

