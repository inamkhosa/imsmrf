## Source directory

h_sources = $(top_srcdir)/include/codec/gsm/gsm.h  $(top_srcdir)/include/codec/gsm/private.h  $(top_srcdir)/include/codec/gsm/proto.h  $(top_srcdir)/include/codec/gsm/unproto.h
cc_sources = add.c code.c debug.c decode.c gsm_create.c gsm_decode.c gsm_destroy.c gsm_encode.c gsm_explode.c gsm_implode.c gsm_option.c gsm_print.c long_term.c lpc.c preprocess.c rpe.c short_term.c table.c

INCLUDES = -I$(top_srcdir)/include/codec/gsm

AC_FLAGS = -DNeedFunctionPrototypes=1 -DSASR -DWAV49 -ansi -pedantic

noinst_LTLIBRARIES= libgsm.la
libgsm_la_SOURCES= $(h_sources) $(cc_sources)
#libgsm_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

