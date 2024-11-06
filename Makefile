all: 
	mkdir -p ./build
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./
	cmake --build ./build

generate:
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./



L1:
	./build/Lab1/Lab1_main

L2:
	./build/Lab2/Lab2_main

L3:
	./build/Lab3/Lab3_main

L4:
	./build/Lab4/Lab4_main

L5:
	./build/Lab5/Lab5_main

L6:
	./build/Lab6/Lab6_main
