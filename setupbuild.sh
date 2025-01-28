#/bin/sh

git submodule init
git submodule update

mkdir build
cd build
cmake ../
