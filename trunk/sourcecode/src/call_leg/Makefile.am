## Source directory

h_sources = $(top_srcdir)/include/call_leg/call_leg_api.h  $(top_srcdir)/include/call_leg/call_leg_define.h
cc_sources = call_leg_api.c  call_leg.c  call_leg.h  call_leg_sm.c  call_leg_sm.h  sip_handler.c  sip_handler.h

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps -I$(includedir)/sipstack -I$(includedir)/dnsresolver $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libcallleg.la
libcallleg_la_SOURCES= $(h_sources) $(cc_sources)
#libcallleg_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

