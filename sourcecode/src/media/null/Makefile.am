## Source directory

h_sources = $(top_srcdir)/include/media/null.h
cc_sources = null_api.c  null_cb.c  null_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libnull.la
libnull_la_SOURCES= $(h_sources) $(cc_sources)
#libnull_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

