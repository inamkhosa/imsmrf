## Source directory

h_sources = $(top_srcdir)/include/codec/g729/basic_op.h \
$(top_srcdir)/include/codec/g729/ld8k.h \
$(top_srcdir)/include/codec/g729/oper_32b.h \
$(top_srcdir)/include/codec/g729/tab_ld8k.h \
$(top_srcdir)/include/codec/g729/typedef.h

cc_sources = acelp_co.c \
basic_op.c \
bits.c \
cod_ld8k.c \
de_acelp.c \
dec_gain.c \
dec_lag3.c \
dec_ld8k.c \
dspfunc.c \
filter.c \
gainpred.c \
lpc.c \
lpcfunc.c \
lspdec.c \
lspgetq.c \
oper_32b.c \
pitch.c \
post_pro.c \
p_parity.c \
pred_lt3.c \
pre_proc.c \
pst.c \
pwf.c \
qua_gain.c \
qua_lsp.c \
tab_ld8k.c \
util.c

INCLUDES = -I$(top_srcdir)/include/codec/g729

noinst_LTLIBRARIES= libg729.la
libg729_la_SOURCES= $(h_sources) $(cc_sources)
#libg711_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

