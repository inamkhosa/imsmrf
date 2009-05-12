## Source directory

h_sources = 
cc_sources = $(top_srcdir)/src/mscml_sm/mscml_sm.h mscml_play_record.c  mscml_play_record.h 

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps -I$(top_srcdir)/src/mscml_sm $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmscml_play_record.la
libmscml_play_record_la_SOURCES= $(h_sources) $(cc_sources)
#libmscml_play_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

