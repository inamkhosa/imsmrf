SUBDIRS = buffer fileread filewrite level mixer null pcmudec pcmuenc pkt rtprecv rtpsend timequeue udprecv udpsend wavread wavwrite gsmdec gsmenc g729enc g729dec reseqbuffer


h_sources = $(top_srcdir)/include/media/buffer.h $(top_srcdir)/include/media/genpkt.h $(top_srcdir)/include/media/pcmuenc.h $(top_srcdir)/include/media/udppkt.h $(top_srcdir)/include/media/codecpkt.h $(top_srcdir)/include/media/level.h $(top_srcdir)/include/media/rtppkt.h $(top_srcdir)/include/media/udprecv.h $(top_srcdir)/include/media/datapkt.h $(top_srcdir)/include/media/media.h $(top_srcdir)/include/media/rtprecv.h $(top_srcdir)/include/media/udpsend.h $(top_srcdir)/include/media/fileread.h $(top_srcdir)/include/media/mixer.h $(top_srcdir)/include/media/rtpsend.h $(top_srcdir)/include/media/wavread.h $(top_srcdir)/include/media/filereadpkt.h $(top_srcdir)/include/media/null.h $(top_srcdir)/include/media/signalpkt.h $(top_srcdir)/include/media/wavreadpkt.h $(top_srcdir)/include/media/filewrite.h $(top_srcdir)/include/media/pcmudec.h $(top_srcdir)/include/media/timequeue.h $(top_srcdir)/include/media/wavwrite.h $(top_srcdir)/include/media/g729enc.h $(top_srcdir)/include/media/g729dec.h $(top_srcdir)/include/media/reseqbuffer.h

cc_sources = media_api.c  media_cb.c  media_priv.h

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS)

noinst_LTLIBRARIES= libmedia.la
libmedia_la_SOURCES= $(h_sources) $(cc_sources)
libmedia_la_LIBADD= buffer/libbuffer.la fileread/libfileread.la filewrite/libfilewrite.la level/liblevel.la mixer/libmixer.la null/libnull.la pcmudec/libpcmudec.la pcmuenc/libpcmuenc.la pkt/libpkt.la rtprecv/librtprecv.la rtpsend/librtpsend.la timequeue/libtimequeue.la udprecv/libudprecv.la udpsend/libudpsend.la wavread/libwavread.la wavwrite/libwavwrite.la gsmdec/libgsmdec.la gsmenc/libgsmenc.la g729enc/libg729enc.la g729dec/libg729dec.la reseqbuffer/libreseqbuffer.la
#libmedia_LDFLAGS= $(UTIL_DEPS_LIBS)
#-version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)


