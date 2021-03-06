#!/bin/sh
set -e -x

unset CONFIG_SITE

INST=`pwd`/inst
rm -rf $INST
mkdir $INST
cd $INST
mkdir lib
ln -s lib lib64
mkdir include
cd ..

UDTHACK=""
if [[ "$GLOBUS_FLAVOR" == *32* && "`getconf LONG_BIT`" == "64" ]]; then
    export CFLAGS="$CFLAGS -m32" 
    export LDFLAGS="$LDFLAGS -m32"
    export CXXFLAGS="$CXXFLAGS -m32"
    UDTHACK=" -m32"
fi

export CFLAGS="$CFLAGS -fPIC -I$INST/include"
export CXXFLAGS="$CXXFLAGS -fPIC -I$INST/include"
export LDFLAGS="$LDFLAGS -L$INST/lib"

export PKG_CONFIG_PATH=$INST/lib/pkgconfig:$PKG_CONFIG_PATH
export PATH=$INST/bin:$PATH

rm -rf libffi-*/
tar xfz libffi-*.tar.gz
cd libffi-*/
./configure --prefix=$INST
make clean all install
cd ..

if [[ "`uname`" == *arwin* ]]; then
    rm -rf gettext-*/
    tar xfz gettext-*.tar.gz
    cd gettext-*/
    ./configure --prefix=$INST
    make clean all install
    cd ..
fi

tar xfz udt.sdk.*.tar.gz 
cd udt4/src
UDTARCH=IA32
UDTOS=LINUX
if [[ "$GLOBUS_FLAVOR" == *64* ]]; then
    UDTARCH=AMD64
fi
if [[ "`uname`" == *arwin* ]]; then
    UDTOS=OSX
fi
make os="$UDTOS$UDTHACK" arch=$UDTARCH clean libudt.a
cp libudt.a $INST/lib
cp udt.h $INST/include
cd ../..

rm -rf glib-*/
tar xfz glib-*.tar.gz
cd glib-*/
./configure --prefix=$INST --enable-static --disable-selinux --disable-xattr --disable-dtrace --enable-debug 
make clean all install
cd ..

rm -rf libnice-*/
tar xfz libnice-*.tar.gz 
cd libnice-*/
./configure --prefix=$INST --disable-shared
make clean all install
cd ..

rm -f inst/lib/*.so inst/lib/*.so.*
if [[ "`uname`" == *arwin* ]]; then
    _BLANK=\"\"
fi

sed -i $_BLANK "/dlname=/ s/'.*'/''/" `find inst/lib -name '*.la'`
sed -i $_BLANK "/library_names=/ s/'.*'/''/" `find inst/lib -name '*.la'`

touch done
