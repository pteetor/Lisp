//
// Tests for garbage collection
//
#include <iostream>

#include "globals.h"
#include "Object.h"
#include "Heap.h"

void report(const char* when, Heap& heap)
{
  cout << when << ": nFreeCells = " << heap.nFreeCells()
       << "  nProtected = " << heap.nProtected() << endl;
}  

int main()
{
  const int N = 5;
  StringSpace space(1000);
  Heap heap(N + 1, &space);

  cout << "Trivial GC" << endl;
  report("before", heap);
  heap.gc();
  report("after", heap);

  cout << endl;
  cout << "One cell, unprotected" << endl;
  cout << "before: nFreeCells = " << heap.nFreeCells() << endl;
  heap.alloc(3.14);
  heap.gc();
  cout << "after: nFreeCells = " << heap.nFreeCells() << endl;

  cout << endl;
  cout << "One cell, protected" << endl;
  report("before", heap);
  Cell *p1 = heap.alloc(10);
  heap.protect(p1);
  report("during", heap);
  heap.gc();
  report("after", heap);

  heap.dump();

  cout << endl;
  cout << "Redundant GC, after needed GC" << endl;
  report("before", heap);
  heap.gc();
  report("after", heap);  

  cout << endl;
  cout << "Two cells, protected" << endl;
  cout << "before: nFreeCells = " << heap.nFreeCells() << endl;
  Cell *p2 = heap.alloc(20);
  heap.protect(p2);
  cout << "during: nFreeCells = " << heap.nFreeCells() << endl;
  heap.gc();
  cout << "after: nFreeCells = " << heap.nFreeCells() << endl;

  cout << endl;
  cout << "Unprotect two cell" << endl;
  report("before", heap);
  heap.unprotect(p1);
  report("during", heap);
  heap.unprotect(p2);
  report("during", heap);
  heap.gc();
  report("after", heap);
}
