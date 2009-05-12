SVN_VERSION=$(shell if test -d .svn ; then svnversion . | sed -e 's/\([1-9]*\)\([MS]\)\?:\?\([1-9]*\)[MS]\?/\1/' ;  else echo unknown ; fi)
VERINFO=-DSVN_VERSION="$(SVN_VERSION)"

subdirs: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
	$(MAKE) -w -C $$dir $(TARGET) || exit 1 ; \
	done

subdirs_clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
	$(MAKE) -w -C $$dir $(TARGET) clean || exit 1 ; \
	done

ARCH=$(shell uname -p|sed s/_/' '/|awk '{print $$2}')

SRCS := $(wildcard *.cc) $(wildcard *.c) $(wildcard *.cpp)
OBJS := $(patsubst %.cc, %.o,$(wildcard *.cc)) $(patsubst %.c, %.o,$(wildcard *.c)) $(patsubst %.cpp, %.o,$(wildcard *.cpp))
DEPS := $(wildcard *.h)


CFLAGS=-g -O0 -Wall -Werror -Wcast-align --no-strict-aliasing $(VERINFO)

LIBS=-L$(TOP)/lib/

CC = gcc

CXX = g++

ifeq ($(LIB_TYPE), static)
	LIBEXT = a
	COMPILE_LIB = $(AR) -ruv $(TOP)/lib/$(LIBNAME).$(LIBEXT) $(OBJS)
else
	LIBEXT = so
	COMPILE_LIB = $(CC) -fPIC -shared -Wl,-soname,$(LIBNAME).$(LIBEXT) -o $(TOP)/lib/$(LIBNAME).$(LIBEXT) $(OBJS) $(LIBS)
endif

