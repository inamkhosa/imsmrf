## Source directory

h_sources = $(top_srcdir)/include/media/gsmdec.h
cc_sources = gsmdec_api.c  gsmdec_cb.c  gsmdec_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libgsmdec.la
libgsmdec_la_SOURCES= $(h_sources) $(cc_sources)
#libgsmdec_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

