#/bin/sh

cd build
while true; do
	cmake --build .
	read -p "Press a key to try again: " -n1 -s
done;
