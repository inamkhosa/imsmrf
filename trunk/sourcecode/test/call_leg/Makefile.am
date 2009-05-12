
bin_PROGRAMS = call_leg_test

call_leg_test_SOURCES = call_leg_test.c  call_leg_test.h

INCLUDES = -I$(top_srcdir)/include $(SDPPARSER_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS)

LIBS = $(SDPPARSER_DEPS_LIBS) $(UTIL_DEPS_LIBS) -lsipstack -lcallleg -L$(top_srcdir)/src/call_leg
#$(SNDFILE_DEPS_LIBS) $(GLIB_DEPS_LIBS) $(LIBXML_DEPS_LIBS) -L$(top_srcdir)/src/call_leg    -lDNSResolver

