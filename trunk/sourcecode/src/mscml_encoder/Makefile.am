## Source directory

h_sources = $(top_srcdir)/include/mscml_encoder/mscml_encoder_api.h
cc_sources = mscml_encoder.c  mscml_encoder.h

INCLUDES = -I$(top_srcdir)/include $(UTIL_DEPS_CFLAGS) $(LIBXML_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmscml_encoder.la
libmscml_encoder_la_SOURCES= $(h_sources) $(cc_sources)
#libmscml_encoder_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

