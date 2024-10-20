all: 
	mkdir -p ./build
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./
	cmake --build ./build

generate:
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./



L1: ./build/Lab1/Lab1_main
	./build/Lab1/Lab1_main

L2: ./build/Lab2/Lab2_main
	./build/Lab2/Lab2_main

