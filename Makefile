all: 
	mkdir -p ./build
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./
	cmake --build ./build


L1: ./build/Lab1/main
	./build/Lab1/main

