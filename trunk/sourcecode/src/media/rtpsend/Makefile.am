## Source directory

h_sources = $(top_srcdir)/include/media/rtpsend.h
cc_sources = rtpsend_api.c  rtpsend_cb.c  rtpsend_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= librtpsend.la
librtpsend_la_SOURCES= $(h_sources) $(cc_sources)
#librtpsend_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

