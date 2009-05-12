## Source directory

h_sources = $(top_srcdir)/include/media/reseqbuffer.h
cc_sources = reseqbuffer_api.c  reseqbuffer_cb.c  reseqbuffer_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= libreseqbuffer.la
libreseqbuffer_la_SOURCES= $(h_sources) $(cc_sources)
#libreseqbuffer_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

