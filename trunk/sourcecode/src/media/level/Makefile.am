## Source directory

h_sources = $(top_srcdir)/include/media/level.h
cc_sources = level_api.c  level.c  level_cb.c  level_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)
# -I$(includedir)/sipstack  $(SDPPARSER_DEPS_CFLAGS)

noinst_LTLIBRARIES= liblevel.la
liblevel_la_SOURCES= $(h_sources) $(cc_sources)
#liblevel_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

