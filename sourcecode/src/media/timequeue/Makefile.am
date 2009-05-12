## Source directory

h_sources = $(top_srcdir)/include/media/timequeue.h
cc_sources = timequeue_api.c  timequeue_cb.c  timequeue_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libtimequeue.la
libtimequeue_la_SOURCES= $(h_sources) $(cc_sources)
#libtimequeue_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

