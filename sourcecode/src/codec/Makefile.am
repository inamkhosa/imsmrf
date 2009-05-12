SUBDIRS = g711 gsm g729


h_sources = 
cc_sources = 

INCLUDES = -I$(top_srcdir)/include

noinst_LTLIBRARIES= libcodec.la
libcodec_la_SOURCES= $(h_sources) $(cc_sources)
libcodec_la_LIBADD= g711/libg711.la gsm/libgsm.la g729/libg729.la
#libcodec_LDFLAGS= $(UTIL_DEPS_LIBS)
#-version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)


