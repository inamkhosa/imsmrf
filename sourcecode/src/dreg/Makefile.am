## Source directory

h_sources = $(top_srcdir)/include/dreg/dreg.h
cc_sources = dreg_api.cpp  dreg.cpp dreg_priv.h  peg_templ.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libdreg.la
libdreg_la_SOURCES= $(h_sources) $(cc_sources)
#libdreg_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

