//
// Simple unit tests for Lisp data structures
//

#include "Heap.h"

// ----------------------------------------------------------

int main() { 
  Heap heap(100);

  cout << endl;
  cout << "heap.nil()->null() = " << heap.nil()->null() << endl;

  cout << endl;
  cout << "heap.nFreeCells() = " << heap.nFreeCells() << endl;
  auto heap3 = heap.alloc(3);
  auto heap4 = heap.alloc(4);
  cout << "heap.nFreeCells() = " << heap.nFreeCells() << endl;
  auto heapCons = heap.cons(heap3, heap4);
  cout << "heap.nFreeCells() = " << heap.nFreeCells() << endl;
  cout << "heapCons->consp() = " << heapCons->consp() << endl;

  cout << endl;
  cout << "Test throwing bad_alloc" << endl;
  Heap uHeap(3);
  cout << "uHeap.nFreeCells() = " << uHeap.nFreeCells() << endl;
  uHeap.alloc(1);
  cout << "uHeap.nFreeCells() = " << uHeap.nFreeCells() << endl;
  uHeap.alloc(2);
  cout << "uHeap.nFreeCells() = " << uHeap.nFreeCells() << endl;
  try {
    uHeap.alloc(3);
  }
  catch (const std::bad_alloc &e) {
    cout << endl << "Caught heap exception" << endl;
  }

  //
  // These next tests use the global heap, not a local heap
  //
  Cell* dots = cons(cons(alloc(3), alloc(2)), alloc(1));

  cout << endl;
  cout << "dots = " << *dots << endl;

  auto xlist = cons(alloc(3), cons(alloc(2), cons(alloc(1), theHeap.nil())));
  cout << "xlist = " << *(xlist) <<  endl;
  
  cout << "length(xlist) = " << length(xlist) << endl;
  cout << "length(xlist->cdr()) = " << length(xlist->cdr()) << endl;
}
