## Source directory

h_sources = $(top_srcdir)/include/ms_debug.h $(top_srcdir)/include/ms_config_api.h $(top_srcdir)/include/ms_api.h
cc_sources = ms_debug.c ms_config_api.c ms_api.c

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(GLIB_DEPS_CFLAGS) $(SOA_DEPS_CFLAGS)

noinst_LTLIBRARIES= libms.la
libms_la_SOURCES= $(h_sources) $(cc_sources)
#libms_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

