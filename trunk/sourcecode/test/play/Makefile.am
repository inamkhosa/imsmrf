
noinst_PROGRAMS = delay_test1 duration_test1 repeat_test1 repeat_test2 repeat_test3 stoponerror_test1 seq_test1

delay_test1_SOURCES = delay_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
duration_test1_SOURCES = duration_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
repeat_test1_SOURCES = repeat_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
repeat_test2_SOURCES = repeat_test2.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
repeat_test3_SOURCES = repeat_test3.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
stoponerror_test1_SOURCES = stoponerror_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
seq_test1_SOURCES = seq_test1.c unit_tests.c mscml_play_tests.h  play_unit_tests.h  test_mscml_play.h  test_play.h unit_tests.h
#_SOURCES = .c

INCLUDES = -I$(top_srcdir)/include $(LIBXML_DEPS_CFLAGS) 

LIBS = $(LIBXML_DEPS_LIBS) $(SNDFILE_DEPS_LIBS) -lamps -lmscml_sm -lmscml_encoder -L$(top_srcdir)/src/mscml_sm -lms -L$(top_srcdir)/src/ms -L$(top_srcdir)/src/mscml_encoder $(UTIL_DEPS_LIBS) -lstdc++ $(GLIB_DEPS_LIBS) -ldreg -L$(top_srcdir)/src/dreg 

