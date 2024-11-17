emulator: 8080.cpp emulator.c
	gcc $^ -o $@

test: 8080.cpp test.cpp
	g++ $^ -lSDL2 -o $@

disassembler: 8080.cpp disassembler.c
	gcc $^ -o $@

window: 8080.cpp window.cpp
	g++ $^ -lSDL2 -o $@

run-emulator: emulator
	./emulator

run-disassembler: disassembler
	./disassembler

run-test: test
	./test

run-window: window
	./window