#!/bin/sh

filepath=$(cd "$(dirname "$0")"; pwd)

cd $filepath
unzip libxls-1.4.0.zip

cd $filepath/libxls
./configure --prefix=/usr
make && make install

cd $filepath
rm -rf libxls

