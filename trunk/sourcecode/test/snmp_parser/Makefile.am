
noinst_PROGRAMS = test_snmp_response1 test_snmp_response2 test_snmp_response_table1 test_snmp_response_table2 test_snmp_request1 test_snmp_request2 test_snmp_request_table1 test_snmp_notification1

test_snmp_response1_SOURCES = test_snmp_response1.c
test_snmp_response2_SOURCES = test_snmp_response2.c
test_snmp_response_table1_SOURCES = test_snmp_response_table1.c
test_snmp_response_table2_SOURCES = test_snmp_response_table2.c
test_snmp_request1_SOURCES = test_snmp_request1.c
test_snmp_request2_SOURCES = test_snmp_request2.c
test_snmp_request_table1_SOURCES = test_snmp_request_table1.c
test_snmp_notification1_SOURCES = test_snmp_notification1.c

INCLUDES = -I$(top_srcdir)/include $(SDPPARSER_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

LDADD = $(UTIL_DEPS_LIBS) $(SDPPARSER_DEPS_LIBS) $(LIBXML_DEPS_LIBS) -lsnmpparser -L$(top_srcdir)/src/snmp_parser

 
