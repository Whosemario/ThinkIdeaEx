#!/bin/sh

mkdir xcode 

rm xcode/* -rf
cd vs2015/x64
cmake ../../ -G Xcode -DCMAKE_BUILD_TYPE=Release -DPYTHON_DIR="C:\\Python27"
cd ../
