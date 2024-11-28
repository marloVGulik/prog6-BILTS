#/bin/sh

cd vendor
git clone https://github.com/ocornut/imgui.git

cd ..
mkdir build
cd build
cmake ../
