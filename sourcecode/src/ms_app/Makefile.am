
bin_PROGRAMS = ms

ms_SOURCES = call_leg_handler.c  ms_main.c  ms_media.c  ms_media_handler.c  ms_mm.c  ms_mscml_handler.c  ms_sm_handler.c  ms_team.c  ms_util.c  non_conf_sip_handler.c call_leg_handler.h  ms_main.h  ms_media.h  ms_media_handler.h  ms_mm.h  ms_mscml_handler.h  ms_sm_handler.h  ms_team.h  ms_util.h  non_conf_sip_handler.h $(top_srcdir)/include/ms_ret_code.h $(top_srcdir)/include/fw_api.h $(top_srcdir)/include/ms_events.h $(top_srcdir)/include/ms.h $(msextra_DATA)

#SVN_VERSION=$(shell if test -d .svn ; then svnversion . | sed -e  's/\([1-9]*\)\([MS]\?\):\?\([1-9]*\)[MS]\?/\1/' ;  else echo unknown ; fi)
#-DSVN_VERSION="$(SVN_VERSION)"

AM_CFLAGS = -DDATADIR=\"$(datadir)/@PACKAGE@-@VERSION@/\" -DCONFIGURE_ARGUMENTS=\"@TOPLEVEL_CONFIGURE_ARGUMENTS@\"

INCLUDES = -I$(top_srcdir)/include -I$(includedir)/amps $(UTIL_DEPS_CFLAGS) $(SDPPARSER_DEPS_CFLAGS) $(SNDFILE_DEPS_CFLAGS) $(GLIB_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS) $(SOA_DEPS_CFLAGS)

LDADD = $(SNDFILE_DEPS_LIBS) $(GLIB_DEPS_LIBS) $(LIBXML_DEPS_LIBS) -lstdc++ -lmscml_sm -lcallleg -lmedia -ldreg -lmscml_encoder -lsiputil -lsdp_oam -lcodec -lrtp -lmssnmp -lsnmpparser -lms \
-L$(top_srcdir)/src/codec \
-L$(top_srcdir)/src/librtp \
-L$(top_srcdir)/src/mscml_sm \
-L$(top_srcdir)/src/call_leg \
-L$(top_srcdir)/src/ms \
-L$(top_srcdir)/src/media \
-L$(top_srcdir)/src/dreg \
-L$(top_srcdir)/src/mscml_encoder \
-L$(top_srcdir)/src/util \
-L$(top_srcdir)/src/snmp \
-L$(top_srcdir)/src/snmp_parser \
$(UTIL_DEPS_LIBS) $(SDPPARSER_DEPS_LIBS) $(SOA_DEPS_LIBS) -lsipstack -lDNSResolver

msextradir =  $(datadir)/ms-@VERSION@
msextra_DATA =  mscml.xsd config.xml local_sdp $(top_srcdir)/sound/*



