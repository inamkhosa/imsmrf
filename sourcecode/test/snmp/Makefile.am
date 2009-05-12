
noinst_PROGRAMS = test_snmp_server1 test_snmp_nserver1 test_snmp_nserver2 test_snmp_get1 test_snmp_get2 test_snmp_set1 test_snmp_set2 test_snmp_nclient1

test_snmp_server1_SOURCES = test_snmp_server1.c

test_snmp_nserver1_SOURCES = test_snmp_nserver1.c

test_snmp_nserver2_SOURCES = test_snmp_nserver2.c

test_snmp_get1_SOURCES = test_snmp_get1.c

test_snmp_get2_SOURCES = test_snmp_get2.c

test_snmp_set1_SOURCES = test_snmp_set1.c

test_snmp_set2_SOURCES = test_snmp_set2.c

test_snmp_nclient1_SOURCES = test_snmp_nclient1.c

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps $(SDPPARSER_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) $(SNDFILE_DEPS_CFLAGS) $(GLIB_DEPS_CFLAGS) $(SOA_DEPS_CFLAGS)

LDADD = $(UTIL_DEPS_LIBS) $(SDPPARSER_DEPS_LIBS) $(LIBXML_DEPS_LIBS) $(SNDFILE_DEPS_LIBS) $(GLIB_DEPS_LIBS) -lamps -lmssnmp -L$(top_srcdir)/src/snmp -lsnmpparser -L$(top_srcdir)/src/snmp_parser -lms -L$(top_srcdir)/src/ms -lmedia -L$(top_srcdir)/src/media -lsiputil -L$(top_srcdir)/src/util -lrtp -L$(top_srcdir)/src/librtp -lcodec -L$(top_srcdir)/src/codec
