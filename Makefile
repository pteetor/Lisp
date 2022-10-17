#
#  Lisp project makefile
#
#  $@ = target
#  $^ = all pre-reqs
#

CPPFLAGS = -g

repl: repl.o Cell.o Heap.o StringSpace.o Reader.o Tokenizer.o functions.o
	g++ -g -o $@ $^

repl.o: repl.cpp Heap.h Object.h Reader.h Tokenizer.h

Cell.o: Cell.cpp Heap.h Object.h

Heap.o: Heap.cpp Heap.h Object.h

Tokenizer.o: Tokenizer.cpp Tokenizer.h

Reader.o: Reader.cpp Reader.h Tokenizer.h Heap.h Object.h

StringSpace.o: StringSpace.cpp globals.h Object.h Heap.h

functions.o: functions.cpp functions.h Object.h Heap.h

#
# tests
#

tests: test-StringSpace test-Cell test-Heap test-Tokenizer test-Reader test-gc

test-Heap: test-Heap.o Heap.o Cell.o StringSpace.o functions.o
	g++ -g -o $@ $^

test-Heap.o: test-Heap.cpp Heap.h Object.h functions.h

test-StringSpace: test-StringSpace.o StringSpace.o Heap.o Cell.o functions.o
	g++ -g -o test-StringSpace $^

test-StringSpace.o: test-StringSpace.cpp Object.h

test-Tokenizer: test-Tokenizer.o Tokenizer.o
	g++ -o $@ $^

test-Tokenizer.o: test-Tokenizer.cpp Tokenizer.h

test-Reader: test-Reader.o Reader.o Tokenizer.o Cell.o Heap.o StringSpace.o functions.o
	g++ -o test-Reader $^

test-Reader.o: test-Reader.cpp Reader.h Tokenizer.h Heap.h 

test-sizes: test-sizes.o Heap.o Cell.o StringSpace.o functions.o
	g++ -g -o $@ $^

test-sizes.o: test-sizes.cpp Heap.h Object.h

test-Cell: test-Cell.o Cell.o StringSpace.o
	g++ -g -o $@ $^

test-Cell.o: test-Cell.cpp Heap.h Object.h

test-gc: test-gc.o Heap.o Cell.o StringSpace.o functions.o
	g++ -g -o $@ $^

test-gc.o: test-gc.cpp globals.h Object.h Heap.h

test-functions: test-functions.o StringSpace.o Heap.o Cell.o functions.o
	g++ -g -o $@ $^

test-functions.o: test-functions.cpp functions.h Object.h Heap.h
