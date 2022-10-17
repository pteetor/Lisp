//
// Tests for garbage collection
//
#include <iostream>

#include "globals.h"
#include "Object.h"
#include "Heap.h"

void report(const char* when, Heap& heap)
{
  cout << when << ": nFreeObjects = " << heap.nFreeObjects()
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
  cout << "before: nFreeObjects = " << heap.nFreeObjects() << endl;
  heap.alloc(3.14);
  heap.gc();
  cout << "after: nFreeObjects = " << heap.nFreeObjects() << endl;

  cout << endl;
  cout << "One cell, protected" << endl;
  report("before", heap);
  Object *p1 = heap.alloc(10);
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
  cout << "before: nFreeObjects = " << heap.nFreeObjects() << endl;
  Object *p2 = heap.alloc(20);
  heap.protect(p2);
  cout << "during: nFreeObjects = " << heap.nFreeObjects() << endl;
  heap.gc();
  cout << "after: nFreeObjects = " << heap.nFreeObjects() << endl;

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
