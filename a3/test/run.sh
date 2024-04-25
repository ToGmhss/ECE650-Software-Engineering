#!/bin/bash

cd ../
if [ -d build ]; then
  rm -r build
fi
cd ../
pwd
cd a3 && mkdir build && cd build && cmake ../
make install
cd ./run/bin
./ece650-a3 
#wd # this is /home/qarthur/git650/qarthur/a3/build/run/bin
#cd ../test
# echo "" > test.log # Clear the log file first
# ../build/rgen > test.log 2>&1 #output cout and cerr both to the log file.