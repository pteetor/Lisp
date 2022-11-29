#
#  Lisp project makefile
#
#  $@ = target
#  $^ = all pre-reqs
#

CPPFLAGS = -g

APPS = repl
TESTS = test-String test-Object test-Heap test-Tokenizer test-Reader test-gc test-StringFinder

HEAP_HDR = globals.h Heap.h ObjPool.h Object.h StringFinder.h

HEAP_OBJ = Heap.o ObjPool.o Object.o StringFinder.o String.o

INTERP_HDR = Interp.h nativeFunctions.h

INTERP_OBJ = Interp.o nativeFunctions.o

apps: $(APPS)

tests: $(TESTS)

all: $(APPS) $(TESTS)

#
# detailed rules
#
repl: repl.o $(HEAP_OBJ) Reader.o Tokenizer.o functions.o $(INTERP_OBJ)
	g++ -g -o $@ $^

repl.o: repl.cpp $(HEAP_HDR) Reader.h Tokenizer.h

Object.o: Object.cpp $(HEAP_HDR)

Heap.o: Heap.cpp $(HEAP_HDR)

Tokenizer.o: Tokenizer.cpp Tokenizer.h

Reader.o: Reader.cpp Reader.h Tokenizer.h Heap.h Object.h

String.o: String.cpp globals.h Object.h Heap.h

## Dict.o: Dict.cpp Dict.h Heap.h Object.h globals.h

functions.o: functions.cpp functions.h Object.h Heap.h

ObjPool.o: ObjPool.cpp ObjPool.h Object.h

StringFinder.o: StringFinder.cpp $(HEAP_HDR)

Interp.o: Interp.cpp $(INTERP_HDR) $(HEAP_HDR)

ConsoleBuffer.o: ConsoleBuffer.cpp ConsoleBuffer.h

nativeFunctions.o: nativeFunctions.cpp nativeFunctions.h $(HEAP_HDR)

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

test-Reader: test-Reader.o Reader.o Tokenizer.o $(HEAP_OBJ) functions.o
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

## test-Dict: test-Dict.o Dict.o $(HEAP_OBJ) functions.o
## 	g++ -g -o $@ $^

test-StringFinder: test-StringFinder.o $(HEAP_OBJ) functions.o
	g++ -g -o $@ $^

test-StringFinder.o: test-StringFinder.cpp $(HEAP_OBJ)

test-symbol: test-symbol.o $(HEAP_OBJ) functions.o
	g++ -g -o $@ $^

test-symbol.o: test-symbol.cpp $(HEAP_HDR)

test-console: test-console.o Tokenizer.o
	g++ -o $@ $^

test-console.o: test-console.cpp Tokenizer.h

test-ConsoleBuffer: test-ConsoleBuffer.o ConsoleBuffer.o
	g++ -o $@ $^

test-eval: test-eval.o $(HEAP_OBJ) $(INTERP_OBJ) functions.o nativeFunctions.o
	g++ -o $@ $^

test-eval.o: test-eval.cpp $(HEAP_HDR) $(INTERP_HDR)
