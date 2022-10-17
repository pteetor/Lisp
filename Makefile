#
#  Lisp project makefile
#
#  $@ = target
#  $^ = all pre-reqs
#

CPPFLAGS = -g

APPS = repl
TESTS = test-String test-Object test-Heap test-Tokenizer test-Reader test-gc

apps: $(APPS)

tests: $(TESTS)

all: $(APPS) $(TESTS)

#
# detailed rules
#
repl: repl.o Object.o Heap.o String.o Reader.o Tokenizer.o functions.o
	g++ -g -o $@ $^

repl.o: repl.cpp Heap.h Object.h Reader.h Tokenizer.h

Object.o: Object.cpp Heap.h Object.h

Heap.o: Heap.cpp Heap.h Object.h

Tokenizer.o: Tokenizer.cpp Tokenizer.h

Reader.o: Reader.cpp Reader.h Tokenizer.h Heap.h Object.h

String.o: String.cpp globals.h Object.h Heap.h

functions.o: functions.cpp functions.h Object.h Heap.h

#
# utility
#
clean:
	/bin/rm $(APPS) $(TESTS) *.o *~

#
# tests
#
test-Heap: test-Heap.o Heap.o Object.o String.o functions.o
	g++ -g -o $@ $^

test-Heap.o: test-Heap.cpp Heap.h Object.h functions.h

test-String: test-String.o String.o Heap.o Object.o functions.o
	g++ -g -o test-String $^

test-String.o: test-String.cpp Object.h

test-Tokenizer: test-Tokenizer.o Tokenizer.o
	g++ -o $@ $^

test-Tokenizer.o: test-Tokenizer.cpp Tokenizer.h

test-Reader: test-Reader.o Reader.o Tokenizer.o Object.o Heap.o String.o functions.o
	g++ -o test-Reader $^

test-Reader.o: test-Reader.cpp Reader.h Tokenizer.h Heap.h 

test-sizes: test-sizes.o Heap.o Object.o String.o functions.o
	g++ -g -o $@ $^

test-sizes.o: test-sizes.cpp Heap.h Object.h

test-Object: test-Object.o Object.o String.o
	g++ -g -o $@ $^

test-Object.o: test-Object.cpp Heap.h Object.h

test-gc: test-gc.o Heap.o Object.o String.o functions.o
	g++ -g -o $@ $^

test-gc.o: test-gc.cpp globals.h Object.h Heap.h

test-functions: test-functions.o String.o Heap.o Object.o functions.o
	g++ -g -o $@ $^

test-functions.o: test-functions.cpp functions.h Object.h Heap.h
