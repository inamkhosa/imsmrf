#! /bin/sh

dir=`echo "$0" | sed 's,[^/]*$,,'`
test "x${dir}" = "x" && dir='.'

if test "x`cd "${dir}" 2>/dev/null && pwd`" != "x`pwd`"
then
    echo "This script must be executed directly from the source directory."
    exit 1
fi

rm -f config.cache acconfig.h

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export CPPFLAGS="-I/usr/include/amps"

echo "- libtoolize."		&& \
libtoolize --force		&& \
echo "- aclocal."		&& \
aclocal				&& \
echo "- autoconf."		&& \
autoconf			&& \
echo "- autoheader."		&& \
autoheader			&& \
echo "- automake."		&& \
automake --add-missing --gnu	&& \
echo				&& \
./configure "$@"		&& exit 0

exit 1

