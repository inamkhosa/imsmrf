
noinst_PROGRAMS = repeat_test1 repeat_test2 repeat_test3 delay_test1 duration_test1 stoponerror_test1 beep_test1 beep_test2 initsilence_test1 dtmfkey_test1 endsilence_test1 recordduration_test1 escapekey_test1

repeat_test1_SOURCES = repeat_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
repeat_test2_SOURCES = repeat_test2.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
repeat_test3_SOURCES = repeat_test3.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
delay_test1_SOURCES = delay_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
duration_test1_SOURCES = duration_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
stoponerror_test1_SOURCES = stoponerror_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
beep_test1_SOURCES = beep_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
beep_test2_SOURCES = beep_test2.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
initsilence_test1_SOURCES = initsilence_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
dtmfkey_test1_SOURCES = dtmfkey_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
endsilence_test1_SOURCES = endsilence_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
recordduration_test1_SOURCES = recordduration_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
escapekey_test1_SOURCES = escapekey_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
#_SOURCES = .c

INCLUDES = -I$(top_srcdir)/include $(LIBXML_DEPS_CFLAGS) $(UTIL_DEPS_CFLAGS)

LIBS = $(GLIB_DEPS_LIBS) $(LIBXML_DEPS_LIBS) $(SNDFILE_DEPS_LIBS) $(UTIL_DEPS_LIBS) -lamps -lmscml_sm -L$(top_srcdir)/src/mscml_sm -lms -L$(top_srcdir)/src/ms -ldreg -L$(top_srcdir)/src/dreg -lstdc++ -lmscml_encoder -L$(top_srcdir)/src/mscml_encoder
