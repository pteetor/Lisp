repl: repl.o Cell.o Heap.o StringPool.o reader.o tokenizer.o
	g++ -o repl repl.o Cell.o StringPool.o Heap.o reader.o tokenizer.o

tests: test-heap test-tokenizer test-reader

test-heap: test-heap.o Cell.o Heap.o StringSpace.o
	g++ -o test-heap test-heap.o Heap.o Cell.o StringSpace.o

test-heap.o: test-heap.cpp Heap.h

test-StringSpace: test-StringSpace.o StringSpace.h StringSpace.o Heap.o Cell.o
	g++ -o test-StringSpace test-StringSpace.o StringSpace.o Heap.o Cell.o

test-StringSpace.o: test-StringSpace.cpp StringSpace.h

test-tokenizer: tokenizer.h test-tokenizer.o tokenizer.o
	g++ -o test-tokenizer test-tokenizer.o tokenizer.o

test-reader:test-reader.o reader.o tokenizer.o Cell.o Heap.o StringSpace.o
	g++ -o test-reader test-reader.o reader.o tokenizer.o Cell.o Heap.o StringSpace.o

test-reader.o: test-reader.cpp Reader.h tokenizer.h Heap.h 

Cell.o: Cell.cpp Heap.h StringSpace.h

Heap.o: Heap.h Heap.cpp

tokenizer.o: tokenizer.h tokenizer.cpp

reader.o: Reader.h reader.cpp tokenizer.h Heap.h

StringSpace.o: StringSpace.cpp globals.h StringSpace.h Heap.h
