//
// Simple unit tests for Lisp data structures
//

#include "interp.h"

// ----------------------------------------------------------

int main() {
  initHeap();
  
  cout << "sizeof(short int) = " << sizeof(short int) << endl;
  cout << "sizeof(int) = " << sizeof(int) << endl;
  cout << "sizeof(long int) = " << sizeof(long int) << endl;
  cout << "sizeof(float) = " << sizeof(float) << endl;
  cout << "sizeof(double) = " << sizeof(double) << endl;
  cout << "sizeof(void *) = " << sizeof(void *) << endl;

  cout << "sizeof(Cell) = " << sizeof(Cell) << endl;
  cout << "sizeof(Cell&) = " << sizeof(Cell&) << endl;

  Cell intCell(3);

  cout << endl;
  cout << "intCell.null() = " << intCell.null() << endl;
  cout << "isAtomic(intCell) = " << intCell.isAtomic() << endl;
  cout << "isCons(intCell) = " << intCell.isCons() << endl;
  cout << "isInt(intCell) = " << intCell.isInt() << endl;
  cout << "(int) intCell = " << (int) intCell << endl;
  cout << "(double) intCell = " << (double) intCell << endl;

  Cell dblCell(3.1415);

  cout << endl;
  cout << "(int) dblCell = " << (int) dblCell << endl;
  cout << "(double) dblCell = " << (double) dblCell << endl;

  Cell consCell(new Cell, new Cell);

  cout << endl;
  cout << "consCell.null() = " << consCell.null() << endl;
  cout << "consCell.isAtomic() = " << consCell.isAtomic() << endl;
  cout << "consCell.isCons() = " << consCell.isCons() << endl;

  try {
    cout << "(int) consCell = " << (int) consCell << endl;
  }
  catch (const std::bad_cast &e) {
    cout << endl << "Caught bad cast" << endl;
  }

  Cell markedCell;
  markedCell.mark();

  cout << endl;
  cout << "intCell.isMarked() = " << intCell.isMarked() << endl;
  cout << "markedCell.isMarked() = " << markedCell.isMarked() << endl;
  
  cout << endl;
  cout << "nil.null() = " << nil->null() << endl;

  Heap heap(100);

  cout << endl;
  cout << "heap.freesize() = " << heap.freesize() << endl;
  auto heap3 = heap.alloc(3);
  auto heap4 = heap.alloc(4);
  cout << "heap.freesize() = " << heap.freesize() << endl;
  auto heapCons = heap.cons(heap3, heap4);
  cout << "heap.freesize() = " << heap.freesize() << endl;
  cout << "heapCons->isCons() = " << heapCons->isCons() << endl;

  Heap uHeap(2);
  uHeap.alloc(1);
  uHeap.alloc(2);
  try {
    uHeap.alloc(3);
  }
  catch (const std::bad_alloc &e) {
    cout << endl << "Caught heap exception" << endl;
  }

  Cell* dots = cons(cons(alloc(3), alloc(2)), alloc(1));

  cout << endl;
  cout << "dots = " << *dots << endl;

  auto xlist = cons(alloc(3), cons(alloc(2), cons(alloc(1), nil)));
  cout << "xlist = " << *(xlist) <<  endl;
  
  cout << "length(xlist) = " << length(xlist) << endl;
  cout << "length(xlist->cdr()) = " << length(xlist->cdr()) << endl;
}
