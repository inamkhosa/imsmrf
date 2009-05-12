## Source directory

h_sources = $(top_srcdir)/include/media/buffer.h
cc_sources = buffer_api.c  buffer_cb.c  buffer_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libbuffer.la
libbuffer_la_SOURCES= $(h_sources) $(cc_sources)
#libbuffer_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

