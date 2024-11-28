@echo off

cd vendor
git clone https://github.com/ocornut/imgui.git

mkdir build
cd build

cmake ../

