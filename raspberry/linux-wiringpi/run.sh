#!/bin/bash

set -e

CUR_DIR=$PWD

./raspberry-dev/init.sh

# cross compile liblouis
export PATH=$PATH:$PWD/raspberry-dev/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin

cd ../liblouis
if [ ! -f configure ]; then
	./autogen.sh
fi
if [ ! -f Makefile ]; then
	./configure --host arm-linux-gnueabihf
fi
make

cd $CUR_DIR

#compile lib + examples
cmake . -DCMAKE_TOOLCHAIN_FILE=./raspberry-dev/toolchain.cmake
make
mkdir -p out_liblouis
cp ../liblouis/liblouis/.libs/liblouis.* out_liblouis/
cp -r ../liblouis/tables out_liblouis/
cp liblouis_install.sh out_liblouis/