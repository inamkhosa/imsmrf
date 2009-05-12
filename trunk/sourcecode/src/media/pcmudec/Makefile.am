## Source directory

h_sources = $(top_srcdir)/include/media/pcmudec.h
cc_sources = pcmudec_api.c  pcmudec_cb.c  pcmudec_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libpcmudec.la
libpcmudec_la_SOURCES= $(h_sources) $(cc_sources)
#libpcmudec_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

