## Source directory

h_sources = $(top_srcdir)/include/media/g729enc.h
cc_sources = g729enc_api.c  g729enc_cb.c  g729enc_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libg729enc.la
libg729enc_la_SOURCES= $(h_sources) $(cc_sources)
#libg729enc_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

