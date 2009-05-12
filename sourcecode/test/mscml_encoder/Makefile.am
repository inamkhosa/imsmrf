
noinst_PROGRAMS = mscml_encoder_test1

mscml_encoder_test1_SOURCES = mscml_encoder_test1.c unit_tests.h

INCLUDES = -I$(top_srcdir)/include $(LIBXML_DEPS_CFLAGS) $(SNDFILE_DEPS_CFLAGS)

LDADD = -lmscml_encoder -L$(top_srcdir)/src/mscml_encoder $(LIBXML_DEPS_LIBS) -lamps $(UTIL_DEPS_LIBS) $(SNDFILE_DEPS_LIBS)

