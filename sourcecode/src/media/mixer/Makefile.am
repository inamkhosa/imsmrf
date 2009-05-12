## Source directory

h_sources = $(top_srcdir)/include/media/mixer.h
cc_sources = mixer_api.c  mixer_cb.c  mixer_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS) 
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmixer.la
libmixer_la_SOURCES= $(h_sources) $(cc_sources)
#libmixer_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

