## Source directory

h_sources = $(top_srcdir)/include/media/pcmuenc.h
cc_sources = pcmuenc_api.c  pcmuenc_cb.c  pcmuenc_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libpcmuenc.la
libpcmuenc_la_SOURCES= $(h_sources) $(cc_sources)
#libpcmuenc_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

