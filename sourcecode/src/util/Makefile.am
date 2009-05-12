## Source directory

h_sources = $(top_srcdir)/include/util/sip_util.h $(top_srcdir)/include/util/sync_timer.h $(top_srcdir)/include/util/circular_list.h
cc_sources = sip_util.c sync_timer.c circular_list.c

INCLUDES = -I$(top_srcdir)/include -I$(includedir) -I$(includedir)/amps $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

noinst_LTLIBRARIES= libsiputil.la
libsiputil_la_SOURCES= $(h_sources) $(cc_sources)
#libsiputil_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
