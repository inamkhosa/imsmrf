## Source directory

h_sources = $(top_srcdir)/include/librtp/rtp.h $(top_srcdir)/include/librtp/rtcp-packet.h $(top_srcdir)/include/librtp/rtp-packet.h
cc_sources = md5c.c  random.c  rtcp-packet.c  rtp.c  rtp-packet.c md5.h  rtp-audio.h 

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= librtp.la
librtp_la_SOURCES= $(h_sources) $(cc_sources)
#librtp_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

