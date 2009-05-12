dnl --------------------------------
dnl Initialization macros.
dnl --------------------------------

AC_INIT([ms],[0.3.1])
AC_PREREQ(2.59)

TOPLEVEL_CONFIGURE_ARGUMENTS=""
AC_SUBST(TOPLEVEL_CONFIGURE_ARGUMENTS)

PACKAGE=ms

MAJOR_VERSION=0
MINOR_VERSION=3
MICRO_VERSION=1

VERSION=$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION
RELEASE=$MAJOR_VERSION.$MINOR_VERSION

PACKAGE_VERSION=$VERSION

AC_SUBST(PACKAGE)
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(VERSION)
AC_SUBST(RELEASE)

AC_CONFIG_SRCDIR([src/ms_app/ms_main.c])
AM_CONFIG_HEADER([config.h])

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)
AC_LANG_CPLUSPLUS

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_SANITY_CHECK

dnl -----------------------------------------------
dnl Checks for libraries.
dnl -----------------------------------------------

PKG_CHECK_MODULES(UTIL_DEPS, libutil-0.3 >= 0.3.0)
AC_SUBST(UTIL_DEPS_CFLAGS)
AC_SUBST(UTIL_DEPS_LIBS)

PKG_CHECK_MODULES(SDPPARSER_DEPS, libsdpparser-0.5 >= 0.5.0)
AC_SUBST(SDPPARSER_DEPS_CFLAGS)
AC_SUBST(SDPPARSER_DEPS_LIBS)

PKG_CHECK_MODULES(SOA_DEPS, libsdp_oam-0.0 >= 0.0.0)
AC_SUBST(SOA_DEPS_CFLAGS)
AC_SUBST(SOA_DEPS_LIBS)

PKG_CHECK_MODULES(SNDFILE_DEPS, sndfile >= 1.0.17)
AC_SUBST(SNDFILE_DEPS_CFLAGS)
AC_SUBST(SNDFILE_DEPS_LIBS)

PKG_CHECK_MODULES(GLIB_DEPS, glib-2.0 >= 2.4.7 )
AC_SUBST(GLIB_DEPS_CFLAGS)
AC_SUBST(GLIB_DEPS_LIBS)

PKG_CHECK_MODULES(LIBXML_DEPS, libxml-2.0 >= 2.6.16)
AC_SUBST(LIBXML_DEPS_CFLAGS)
AC_SUBST(LIBXML_DEPS_LIBS)

AC_CHECK_LIB(amps,[AMPS_Init], [], [AC_MSG_ERROR([AMPS library not found])])
#AC_CHECK_LIB(sipstack,[SIP_Init], [], [AC_MSG_ERROR([SIP library not found])])
AC_CHECK_LIB(DNSResolver,[DNSResolver_ModuleInit], [], [AC_MSG_ERROR([DNSResolver library not found])])

AC_ARG_ENABLE(debug,[AS_HELP_STRING([--enable-debug],[Add compiler debugging options @<:@default=no@:>@])], [enable_debug=$enableval], [enable_debug=no])

AC_ARG_ENABLE(media-trace,[AS_HELP_STRING([--enable-media-trace],[Display media traces @<:@default=no@:>@])], [enable_media_trace=$enableval], [enable_media_trace=no])

AC_ARG_ENABLE(ms-assert,[AS_HELP_STRING([--enable-ms-assert],[Define ms_assert*() macros @<:@default=no@:>@])], [enable_ms_assert=$enableval], [enable_ms_assert=no])

AC_ARG_ENABLE(ms-trace,[AS_HELP_STRING([--enable-ms-trace],[Define tracing @<:@default=no@:>@])], [enable_trace=$enableval], [enable_trace=no])

AC_ARG_ENABLE(assert,[AS_HELP_STRING([--enable-assert],[Define system assert() macro @<:@default=no@:>@])], [enable_assert=$enableval], [enable_assert=no])

AC_ARG_ENABLE(opt,[AS_HELP_STRING([--enable-opt],[Define compiler optimization level (applicable if debug disabled)@<:@default=0@:>@])], [opt_level=$enableval], [opt_level=0])

AC_ARG_ENABLE(snmp,[AS_HELP_STRING([--enable-snmp],[SNMP support @<:@default=yes@:>@])], [enable_snmp=$enableval], [enable_snmp=yes])

AC_ARG_ENABLE(mode,[AS_HELP_STRING([--enable-mode],[Compile for release/debug @<:@default=release@:>@])], [enable_mode=$enableval], [enable_mode=none])

CFLAGS=""
AS_IF([test "x$enable_mode" == xrelease],[enable_debug=no;opt_level=8;enable_ms_assert=no;enable_assert=no;enable_trace=no;enable_snmp=yes])
AS_IF([test "x$enable_mode" == xdebug],[enable_debug=yes;opt_level=0;enable_ms_assert=yes;enable_assert=yes;enable_trace=yes;enable_snmp=yes])
AS_IF([test "x$enable_debug" == xyes],[CFLAGS="$CFLAGS -g -O0"])
AS_IF([test "x$enable_media_trace" == xyes],[AC_DEFINE([ENABLE_MEDIA_TRACE], [1], [Media traces])])
AS_IF([test "x$enable_ms_assert" == xyes],[AC_DEFINE([MS_DEBUG], [], [ms_assert*() macros])])
AS_IF([test "x$enable_trace" == xyes],[AC_DEFINE([MS_TRACE], [TRACE], [trace macros])], [AC_DEFINE([MS_TRACE], [], [trace macros])])
AS_IF([test "x$enable_assert" == xno],[AC_DEFINE([NDEBUG], [], [assert() macro])])
AS_IF([test "x$opt_level" != "x0"],[CFLAGS="$CFLAGS -O$opt_level"])
AS_IF([test "x$enable_snmp" == xyes],[AC_DEFINE([ENABLE_SNMP], [1], [SNMP support])])
CXXFLAGS=$CFLAGS


dnl -----------------------------------------------
dnl Checks for header files.
dnl -----------------------------------------------

AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([sys/time.h])

AC_CHECK_HEADERS([amps/AMPS_API.h amps/AMPS_Defines.h], [], [AC_MSG_ERROR([AMPS header files not found])])
AC_CHECK_HEADERS([sipstack/SIP_API.h], [], [AC_MSG_ERROR([SIP header files not found])])


dnl -----------------------------------------------
dnl Checks for typedefs, structures, and compiler characteristics.
dnl -----------------------------------------------


AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T


dnl -----------------------------------------------
dnl Checks for functions.
dnl -----------------------------------------------

AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([inet_ntoa])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([strcasecmp])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strncasecmp])
AC_CHECK_FUNCS([strtol])
AC_CHECK_FUNCS([strtoul])
AC_CHECK_FUNCS([uname])

AC_FUNC_FORK
AC_FUNC_MEMCMP
AC_FUNC_STAT

dnl -----------------------------------------------
dnl Generates Makefiles.
dnl -----------------------------------------------

AC_CONFIG_FILES(conf/Doxyfile \
		src/ms_app/config.xml \
		ms.spec \
		Makefile \
		src/Makefile \
		src/util/Makefile \
		src/call_leg/Makefile \
		src/ms/Makefile \
		src/librtp/Makefile \
		src/mscml_sm/Makefile \
		src/dreg/Makefile \
		src/mscml_sm/play/Makefile \
		src/mscml_sm/play_collect/Makefile \
		src/mscml_sm/play_record/Makefile \
		src/mscml_encoder/Makefile \
		src/codec/Makefile \
		src/codec/g711/Makefile \
		src/codec/gsm/Makefile \
		src/codec/g729/Makefile \
		src/media/Makefile \
		src/media/buffer/Makefile \
		src/media/fileread/Makefile \
		src/media/filewrite/Makefile \
		src/media/level/Makefile \
		src/media/mixer/Makefile \
		src/media/null/Makefile \
		src/media/pcmudec/Makefile \
		src/media/pcmuenc/Makefile \
		src/media/pkt/Makefile \
		src/media/rtprecv/Makefile \
		src/media/rtpsend/Makefile \
		src/media/timequeue/Makefile \
		src/media/udprecv/Makefile \
		src/media/udpsend/Makefile \
		src/media/wavread/Makefile \
		src/media/wavwrite/Makefile \
		src/media/gsmdec/Makefile \
		src/media/gsmenc/Makefile \
		src/media/g729enc/Makefile \
		src/media/g729dec/Makefile \
		src/media/reseqbuffer/Makefile \
		src/snmp_parser/Makefile \
		src/snmp/Makefile \
		src/ms_app/Makefile \
		test/Makefile \
		test/dreg/Makefile \
		test/media/Makefile \
		test/mscml_encoder/Makefile \
		test/play/Makefile \
		test/play_record/Makefile \
		test/play_collect/Makefile \
		test/snmp_parser/Makefile \
		test/util/Makefile \
		test/snmp/Makefile
)

dnl -----------------------------------------------
dnl Generates Makefiles.
dnl -----------------------------------------------

AC_OUTPUT

