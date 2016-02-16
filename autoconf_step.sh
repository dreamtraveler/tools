#autoscan
aclocal
autoheader
libtoolize --automake --copy --debug --force
autoconf
automake -a -c
./configure
