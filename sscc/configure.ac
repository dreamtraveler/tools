AC_INIT([sscc], [0.1])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_LIBTOOL

export CXXFLAGS="$CXXFLAGS -std=c++11 -O2 -Wall -I. -I.. "
export CXXFLAGS="$CXXFLAGS -I/usr/local/include/luajit-2.0"
export CXXFLAGS="$CXXFLAGS -D__GX_SERVER__"
export LDFLAGS="$LDFLAGS -L../libgx -lluajit-5.1 -lrt -lm -ldl -lz -lmysqlclient"

AC_CONFIG_SUBDIRS([
	../libgx
])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

