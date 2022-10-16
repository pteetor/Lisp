//
// Simple unit tests for Lisp data structures
//

#include "globals.h"
#include "StringSpace.h"
#include "Heap.h"
#include "functions.h"

// ----------------------------------------------------------

int main() {
  StringSpace sSpace(1000);
  Heap heap(1000, &sSpace);

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
  StringSpace uSpace(10);
  Heap uHeap(3, &uSpace);
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
  Cell* dots = heap.cons(heap.cons(heap.alloc(3), heap.alloc(2)), heap.alloc(1));

  cout << endl;
  cout << "dots = " << *dots << endl;

  auto xlist = heap.cons(heap.alloc(3), heap.cons(heap.alloc(2), heap.cons(heap.alloc(1), heap.nil())));
  cout << "xlist = " << *(xlist) <<  endl;
  
  cout << "length(xlist) = " << length(xlist) << endl;
  cout << "length(xlist->cdr()) = " << length(xlist->cdr()) << endl;
}
