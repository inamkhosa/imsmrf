
noinst_PROGRAMS = repeat_test1 repeat_test2 repeat_test3 delay_test1 duration_test1 stoponerror_test1 firstdigit_timer_test1  interdigit_timer_test1 extradigit_timer_test1 returnkey_test1 escapekey_test1 no_maxdigit_and_regex_test1 both_maxdigit_regex_test1 dtmfkey_test1

repeat_test1_SOURCES = repeat_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
repeat_test2_SOURCES = repeat_test2.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
repeat_test3_SOURCES = repeat_test3.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
delay_test1_SOURCES = delay_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
duration_test1_SOURCES = duration_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
stoponerror_test1_SOURCES = stoponerror_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
firstdigit_timer_test1_SOURCES = firstdigit_timer_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
interdigit_timer_test1_SOURCES = interdigit_timer_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
extradigit_timer_test1_SOURCES = extradigit_timer_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
returnkey_test1_SOURCES = returnkey_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
escapekey_test1_SOURCES = escapekey_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
no_maxdigit_and_regex_test1_SOURCES = no_maxdigit_and_regex_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
both_maxdigit_regex_test1_SOURCES = both_maxdigit_regex_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h
dtmfkey_test1_SOURCES = dtmfkey_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h  unit_tests.h


INCLUDES = -I$(top_srcdir)/include $(LIBXML_DEPS_CFLAGS)

LIBS = $(GLIB_DEPS_LIBS) $(LIBXML_DEPS_LIBS) $(SNDFILE_DEPS_LIBS) $(UTIL_DEPS_LIBS) -lamps -lmscml_sm -L$(top_srcdir)/src/mscml_sm -lms -L$(top_srcdir)/src/ms -ldreg -L$(top_srcdir)/src/dreg -lstdc++ -lmscml_encoder -L$(top_srcdir)/src/mscml_encoder

