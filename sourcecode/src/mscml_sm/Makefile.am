SUBDIRS = play play_collect play_record
# play_record

h_sources = mscml_sm.h $(top_srcdir)/include/mscml_sm/mscml_sm_api.h
cc_sources = mscml_sm.c mscml_sm_api.c

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmscml_sm.la
libmscml_sm_la_SOURCES= $(h_sources) $(cc_sources)
libmscml_sm_la_LIBADD= play/libmscml_play.la play_collect/libmscml_play_collect.la play_record/libmscml_play_record.la


