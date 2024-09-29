all: buildDir generate
	cmake --build ./build

generate: ./CMakeLists.txt ./Lab1/CMakeLists.txt
	cmake -S . -B ./build/ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	mv ./build/compile_commands.json ./

buildDir:
	mkdir -p ./build
