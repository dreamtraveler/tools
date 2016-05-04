#autoscan
aclocal
autoheader
libtoolize --automake --copy --debug --force
autoconf
automake -a -c
./configure --prefix=/home/tanlei/learning/deps/bin
