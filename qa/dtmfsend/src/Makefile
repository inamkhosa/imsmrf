TOP=..

all: dtmfsend

include $(TOP)/Makefile.inc

CFLAGS+=`pkg-config --cflags gstreamer-base-0.10` `pkg-config --cflags glib-2.0` `pkg-config --cflags libxml-2.0`

LIBS+=`pkg-config --libs gstreamer-base-0.10` `pkg-config --libs glib-2.0` `pkg-config --libs libxml-2.0`

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) $<

dtmfsend: dtmfsend.o
	$(CC) -o $(TOP)/bin/dtmfsend dtmfsend.o $(LIBS)
clean: 
	$(RM) *~
	$(RM) $(OBJS)
	$(RM) $(TOP)/bin/dtmfsend


