#
#  Lisp project makefile
#
#  $@ = target
#  $^ = all pre-reqs
#

CPPFLAGS = -g

repl: repl.o Cell.o Heap.o StringSpace.o reader.o tokenizer.o
	g++ -g -o $@ $^

repl.o: repl.cpp Heap.h StringSpace.h Reader.h tokenizer.h

Cell.o: Cell.cpp Heap.h StringSpace.h

Heap.o: Heap.cpp Heap.h StringSpace.h

tokenizer.o: tokenizer.cpp tokenizer.h

reader.o: reader.cpp Reader.h tokenizer.h Heap.h

StringSpace.o: StringSpace.cpp globals.h StringSpace.h Heap.h

functions.o: functions.cpp functions.h StringSpace.h Heap.h

#
# tests
#

tests: test-StringSpace test-Cell test-heap test-tokenizer test-reader test-gc

test-heap: test-heap.o Heap.o Cell.o StringSpace.o functions.o
	g++ -g -o $@ $^

test-heap.o: test-heap.cpp Heap.h StringSpace.h functions.h

test-StringSpace: test-StringSpace.o StringSpace.h StringSpace.o Heap.o Cell.o
	g++ -g -o test-StringSpace @^

test-StringSpace.o: test-StringSpace.cpp StringSpace.h

test-tokenizer: test-tokenizer.o tokenizer.o
	g++ -o $@ $^

test-tokenizer.o: test-tokenizer.cpp tokenizer.h

test-reader: test-reader.o reader.o tokenizer.o Cell.o Heap.o StringSpace.o
	g++ -o test-reader $^

test-reader.o: test-reader.cpp Reader.h tokenizer.h Heap.h 

test-sizes: test-sizes.o Heap.o Cell.o StringSpace.o
	g++ -g -o $@ $^

test-sizes.o: test-sizes.cpp Heap.h StringSpace.h

test-Cell: test-Cell.o Cell.o StringSpace.o
	g++ -g -o $@ $^

test-Cell.o: test-Cell.cpp Heap.h StringSpace.h

test-gc: test-gc.o Heap.o Cell.o StringSpace.o functions.o
	g++ -g -o $@ $^

test-gc.o: test-gc.cpp globals.h StringSpace.h Heap.h

test-functions: test-functions.o StringSpace.o Heap.o Cell.o functions.o
	g++ -g -o $@ $^

test-functions.o: test-functions.cpp functions.h StringSpace.h Heap.h
