#!/bin/sh

#mkdir vs2015/x64 -p

#rm vs2015/x64/* -rf
cd vs2015/x64
cmake ../../.. -G"Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release -DBOOST_DIR="D:\open-source\boost_1_61_0"
cd ../..
