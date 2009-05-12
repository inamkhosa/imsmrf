## Source directory

h_sources = $(top_srcdir)/include/snmp/snmp_comm.h $(top_srcdir)/include/snmp/snmp_defines.h snmp_comm_priv.h
cc_sources = snmp_comm.c snmp_comm_priv.c

INCLUDES = -I$(top_srcdir)/include -I$(includedir) -I$(includedir)/amps $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) $(GLIB_DEPS_CFLAGS) $(SOA_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmssnmp.la
libmssnmp_la_SOURCES= $(h_sources) $(cc_sources)
#libmssnmp_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
