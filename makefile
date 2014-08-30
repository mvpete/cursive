
all: mark-down

mark-down: main.o node.o document.o state_machine.o diagnostics.o
	g++ -std=c++11 -I ./ -o mark-down bin/main.o bin/node.o bin/document.o bin/diagnostics.o bin/state_machine.o
tests: node.o document.o tests.o state_machine.o diagnostics.o
	g++ -std=c++11 -o md-tests bin/node.o bin/document.o bin/diagnostics.o bin/tests.o bin/state_machine.o

tests.o: src/tests.cpp
	g++ -std=c++11 -c -I ./ -o bin/tests.o src/tests.cpp

node.o: inc/node.h src/node.cpp
	g++ -std=c++11 -c -I ./ -o bin/node.o src/node.cpp
document.o: src/document.cpp inc/document.h
	g++ -std=c++11 -c -I ./ -o bin/document.o src/document.cpp
main.o: src/main.cpp
	g++ -std=c++11 -c -I ./ -o bin/main.o src/main.cpp

diagnostics.o: src/diagnostics.cpp
	g++ -std=c++11 -c -I ./ -o bin/diagnostics.o src/diagnostics.cpp

state_machine.o: src/state_machine.cpp
	g++ -std=c++11 -c -I ./ -o bin/state_machine.o src/state_machine.cpp