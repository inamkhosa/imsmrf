## Source directory

h_sources = $(top_srcdir)/include/media/udpsend.h
cc_sources = udpsend_api.c  udpsend_cb.c  udpsend_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libudpsend.la
libudpsend_la_SOURCES= $(h_sources) $(cc_sources)
#libudpsend_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

