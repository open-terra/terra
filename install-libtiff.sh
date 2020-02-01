#!/bin/sh
wget https://download.osgeo.org/libtiff/tiff-4.1.0.tar.gz
tar -xzf tiff-4.1.0.tar.gz
mkdir tiff-4.1.0/build
cd tiff-4.1.0/build
./configure
make && sudo make install
