//
// Implementation of Heap object class
//
#include <iostream>

#include "StringSpace.h"
#include "Heap.h"

//
// Global variable definitions
//
Heap theHeap(1000);

// ----------------------------------------------------------

//
// Heap - private methods
//

Heap::Heap(int n) {
  nCells = n;
  heap = new Cell[n];

  pFree = nil();
  nFree = 0;

  // Careful here: Don't 'free' the nil cell
  while (--n > 0) {
    free(&heap[n]);
  }

  pProtected = nil();
}

Heap::~Heap() {
  delete heap;
}

Cell* Heap::alloc() {
  if (pFree->eq(nil())) throw std::bad_alloc();

  // Double-check:
  if (nFree == 0) throw std::bad_alloc();

  auto p = pFree;
  pFree = pFree->cdr();

  nFree--;
  return p;
}

void Heap::free(Cell* p)
{
  p->replacd(pFree);
  pFree = p;
  ++nFree;
}

void Heap::protect(Cell* p)
{
  pProtected = cons(p, pProtected);
}

// Mark all protected cells
void Heap::mark()
{
  // Nil is implicitly protected!
  nil()->mark();

  // The protected list is implicitly protected!
  Cell* p = pProtected;
  
  while (p->neq(nil())) {
    p->mark();
    mark(p->car());
    p = p->cdr();
  }
}

// Mark specific cells
void Heap::mark(Cell* p)
{
  if (p->isMarked())
    return;
  
  while (p->consp()) {
    p->mark();
    mark(p->car());
    p = p->cdr();
    if (p->isMarked())
      return;
  }

  // Here, *p is an unmarked atom
  p->mark();
}

void Heap::gc()
{
  mark();
  sweep();

  // TODO: Compactify string space
}

// Free all unmarked cells
void Heap::sweep() {
  for (int i = 0; i < nCells; ++i) {
    if (heap[i].isMarked())
      heap[i].unmark();
    else
      free(&heap[i]);
  }
}

//
// Heap - public methods
//

Cell* Heap::alloc(bool b) { return alloc()->set(b); }
Cell* Heap::alloc(char c) { return alloc()->set(c); }
Cell* Heap::alloc(int i) { return alloc()->set(i); }
Cell* Heap::alloc(double d) { return alloc()->set(d); }
Cell* Heap::alloc(const char *s) { return alloc()->set(s); }
Cell* Heap::allocSymbol(const char *s) { return alloc()->setSymbol(s); }

Cell* Heap::cons(Cell* a, Cell* d) { return alloc()->set(a, d); }



void Heap::dump()
{
    cout << "--- start heap ---" << endl;
    cout << "nFree = " << nFree
	 << "; pFree = " << std::hex << (long int) pFree << endl;
    for (int i = 0; i < nCells; ++i) {
      cout << std::dec << i << " (" << std::hex << (long int) &heap[i] << "): "
	   << std::hex << (long int) (heap[i].car()) << ", "
	   << std::hex << (long int) (heap[i].cdr()) << endl;
    }
    cout << "--- end   heap ---" << endl;
}

// ----------------------------------------------------------

//
// Global, static functions for coding convenience
//

Cell* alloc(bool b) { return theHeap.alloc(b); }
Cell* alloc(char c) { return theHeap.alloc(c); }
Cell* alloc(int i) { return theHeap.alloc(i); }
Cell* alloc(double d) { return theHeap.alloc(d); }

Cell* alloc(const char* s)
{
  // TODO: Check for duplicate string in heap

  return theHeap.alloc(s);
}

Cell* cons(Cell* a, Cell* d) { return theHeap.cons(a, d); }

