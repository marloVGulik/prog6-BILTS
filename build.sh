#/bin/sh

cd build
while true; do
	rm ./bin/BILTS
	cmake --build .
	if [ -e ./bin/BILTS ]
	then
		./bin/BILTS
	else
		echo "No binary, not running!"
	fi

	read -p "Press a key to try again: " -n1 -s
	clear
done;
