//
// Global variables for Lisp interpreter
//
#include "interp.h"

Cell* nil;
CellHeap theHeap(1000);

void initHeap() {
  nil = theHeap.nil();
}

Cell* alloc(bool b) { return theHeap.alloc(b); }
Cell* alloc(char c) { return theHeap.alloc(c); }
Cell* alloc(int i) { return theHeap.alloc(i); }
Cell* alloc(double d) { return theHeap.alloc(d); }

Cell* cons(Cell* a, Cell* d) { return theHeap.cons(a, d); }

