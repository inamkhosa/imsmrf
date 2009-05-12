## Source directory

h_sources = $(top_srcdir)/include/media/codecpkt.h  $(top_srcdir)/include/media/filereadpkt.h  $(top_srcdir)/include/media/rtppkt.h     $(top_srcdir)/include/media/udppkt.h $(top_srcdir)/include/media/datapkt.h   $(top_srcdir)/include/media/genpkt.h       $(top_srcdir)/include/media/signalpkt.h  $(top_srcdir)/include/media/wavreadpkt.h
cc_sources = codecpkt.c  datapkt.c  filereadpkt.c  genpkt.c  rtppkt.c  signalpkt.c  udppkt.c  wavreadpkt.c

INCLUDES = -I$(top_srcdir)/include $(GLIB_DEPS_CFLAGS)

noinst_LTLIBRARIES= libpkt.la
libpkt_la_SOURCES= $(h_sources) $(cc_sources)
#libpkt_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

