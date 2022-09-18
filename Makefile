tests: test-obj test-tokenizer test-reader

test-obj: interp.h test-obj.o StringPool.o Cell.o Heap.o
	g++ -o test-obj test-obj.o StringPool.o Cell.o Heap.o

test-tokenizer: tokenizer.h test-tokenizer.o tokenizer.o
	g++ -o test-tokenizer test-tokenizer.o tokenizer.o

test-reader: interp.h tokenizer.h reader.h test-reader.o reader.o tokenizer.o StringPool.o Cell.o Heap.o reader.o 
	g++ -o test-reader test-reader.o reader.o tokenizer.o StringPool.o Cell.o Heap.o

StringPool.o: interp.h StringPool.cpp

Cell.o: interp.h Cell.cpp

Heap.o: interp.h Heap.cpp

tokenizer.o: tokenizer.h tokenizer.cpp
