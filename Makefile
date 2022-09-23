repl: repl.o Cell.o Heap.o StringPool.o reader.o tokenizer.o
	g++ -o repl repl.o Cell.o StringPool.o Heap.o reader.o tokenizer.o

tests: test-heap test-tokenizer test-reader

test-heap: Heap.h test-heap.o StringPool.o Cell.o Heap.o
	g++ -o test-heap test-heap.o StringPool.o Cell.o Heap.o

test-tokenizer: tokenizer.h test-tokenizer.o tokenizer.o
	g++ -o test-tokenizer test-tokenizer.o tokenizer.o

test-reader: Heap.h tokenizer.h Reader.h test-reader.o reader.o tokenizer.o StringPool.o Cell.o Heap.o reader.o 
	g++ -o test-reader test-reader.o reader.o tokenizer.o StringPool.o Cell.o Heap.o

StringPool.o: Heap.h StringPool.cpp

Cell.o: Heap.h Cell.cpp

Heap.o: Heap.h Heap.cpp

tokenizer.o: tokenizer.h tokenizer.cpp

reader.o: Reader.h reader.cpp tokenizer.h Heap.h
