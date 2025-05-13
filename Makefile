build/tobas: sources/main.cpp
	g++ -o build/tobas sources/main.cpp -lluajit -lraylib -lode -Iinclude -Llib
