emulator: emulator.c
	gcc $^ -o $@

test: test.cpp
	g++ $^ -lSDL2 -o $@

disassembler: disassembler.c
	gcc $^ -o $@

window: window.cpp
	g++ $^ -lSDL2 -o $@

run-emulator: emulator
	./emulator

run-disassembler: disassembler
	./disassembler

run-test: test
	./test

run-window: window
	./window