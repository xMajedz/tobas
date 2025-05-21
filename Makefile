linux: build/tobas

windows: build/tobas.exe

build/tobas: sources/main.cpp
	g++ sources/main.cpp -lluajit -lraylib -lode -Iinclude -Llib -o build/tobas
build/tobas.exe: sources/main.cpp
	g++ sources/main.cpp -lluajit -lraylib -lode -lwinmm -lgdi32 -lopengl32 -Iinlucde -Llib -o build/tobas.exe
