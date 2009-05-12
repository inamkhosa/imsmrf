## Source directory

h_sources = 
cc_sources = $(top_srcdir)/src/mscml_sm/mscml_sm.h mscml_play.h mscml_play.c mscml_play_onentry.c mscml_play_onexit.c

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src/mscml_sm $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) -I$(includedir)/sipstack -I$(includedir)/amps

noinst_LTLIBRARIES= libmscml_play.la
libmscml_play_la_SOURCES= $(h_sources) $(cc_sources)
#libmscml_play_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

