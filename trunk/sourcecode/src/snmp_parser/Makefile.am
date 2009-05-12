## Source directory

h_sources = $(top_srcdir)/include/snmp_parser/snmp_parser_api.h snmp_priv.h
cc_sources = snmp_parser_api.c snmp.c

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/include/snmp_parser $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

noinst_LTLIBRARIES= libsnmpparser.la
libsnmpparser_la_SOURCES= $(h_sources) $(cc_sources)
#libsnmpparser_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
