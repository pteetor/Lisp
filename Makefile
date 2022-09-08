tests: test-obj test-tokenizer test-reader

test-obj: interp.h test-obj.cpp StringPool.o Cell.o CellHeap.o
	g++ -o test-obj test-obj.cpp StringPool.o Cell.o CellHeap.o

test-tokenizer: tokenizer.h test-tokenizer.cpp
	g++ -o test-tokenizer test-tokenizer.cpp

test-reader: interp.h tokenizer.h reader.h test-reader.cpp StringPool.o Cell.o CellHeap.o reader.o 
	g++ -o test-reader test-reader.cpp reader.o StringPool.o Cell.o CellHeap.o

test-StringSrc: CharSrc.h CharSrc.o test-StringSrc.cpp
	g++ -o test-StringSrc test-StringSrc.cpp CharSrc.o

StringPool.o: interp.h StringPool.cpp

Cell.o: interp.h Cell.cpp

CellHeap.o: interp.h CellHeap.cpp

CharSrc.o: CharSrc.h CharSrc.cpp
