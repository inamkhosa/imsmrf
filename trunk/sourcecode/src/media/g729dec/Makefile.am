## Source directory

h_sources = $(top_srcdir)/include/media/g729dec.h
cc_sources = g729dec_api.c  g729dec_cb.c  g729dec_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libg729dec.la
libg729dec_la_SOURCES= $(h_sources) $(cc_sources)
#libg729dec_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

