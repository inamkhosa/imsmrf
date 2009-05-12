## Source directory

h_sources = $(top_srcdir)/include/media/rtprecv.h
cc_sources = rtprecv_api.c  rtprecv_cb.c  rtprecv_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= librtprecv.la
librtprecv_la_SOURCES= $(h_sources) $(cc_sources)
#librtprecv_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

