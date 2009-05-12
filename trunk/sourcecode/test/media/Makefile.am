noinst_PROGRAMS = test_udprecv test_udpsend test_rtprecv test_rtpsend test_fileread test_pcmuenc test_pcmudec test_timequeue test_buffer test_filewrite test_level test_media_api1 test_level2 test_wavwrite test_wavread test_mixer1 test_gsmenc test_gsmdec test_g729enc test_g729dec test_reseqbuffer

#test_mixer2 test_mixer3

test_udprecv_SOURCES = test_udprecv.c
test_udpsend_SOURCES = test_udpsend.c
test_rtprecv_SOURCES = test_rtprecv.c
test_rtpsend_SOURCES = test_rtpsend.c
test_fileread_SOURCES = test_fileread.c
test_pcmuenc_SOURCES = test_pcmuenc.c
test_pcmudec_SOURCES = test_pcmudec.c
test_timequeue_SOURCES =  test_timequeue.c
test_buffer_SOURCES = test_buffer.c
test_filewrite_SOURCES = test_filewrite.c
test_level_SOURCES = test_level.c
test_media_api1_SOURCES = test_media_api1.c
test_level2_SOURCES = test_level2.c
test_wavwrite_SOURCES = test_wavwrite.c
test_mixer1_SOURCES = test_mixer1.c
test_wavread_SOURCES = test_wavread.c
test_gsmenc_SOURCES = test_gsmenc.c
test_gsmdec_SOURCES = test_gsmdec.c
test_g729enc_SOURCES = test_g729enc.c
test_g729dec_SOURCES = test_g729dec.c
test_reseqbuffer_SOURCES = test_reseqbuffer.c

#test_mixer2_SOURCES = test_mixer2.c
#test_mixer3_SOURCES = test_mixer3.c
#_SOURCES = .c

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS) $(SNDFILE_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS)

LDADD = $(GLIB_DEPS_LIBS) -lamps -lms -L$(top_srcdir)/src/ms -lmedia -L$(top_srcdir)/src/media/ $(SNDFILE_DEPS_LIBS) -lrtp -L$(top_srcdir)/src/librtp -lcodec -L$(top_srcdir)/src/codec $(UTIL_DEPS_LIBS) -lsiputil -L$(top_srcdir)/src/util/
