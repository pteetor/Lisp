//
// Implementation of Heap object class
//
#include <iostream>

#include "StringSpace.h"
#include "Heap.h"
#include "functions.h"

//
// Heap - private methods
//

Heap::Heap(int n, StringSpace *ss) {
  nCells = n;
  heap = new Cell[n];
  strings = ss;

  heap[0].setNil(); 
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
  p->setFree();
  p->replacd(pFree);
  pFree = p;
  ++nFree;
}

void Heap::protect(Cell* p)
{
  pProtected = cons(p, pProtected);
}

void Heap::unprotect(Cell* p)
{
  pProtected = elide(pProtected, p);
}

int Heap::nProtected()
{
  return length(pProtected);
}

// Mark all protected cells
void Heap::mark()
{
  // Nil is implicitly protected!
  nil()->mark();

  // The protected list is implicitly protected!
  Cell* p = pProtected;
  Cell* q;
  
  while (p->neq(nil())) {
    q = p->car();   // Save car before mark() tramples on it
    p->mark();
    mark(q);
    p = p->cdr();
  }
}

// Mark specific cells
void Heap::mark(Cell* p)
{
  if (p->isMarked())
    return;

  Cell* q;
  
  while (p->consp()) {
    q = p->car();   // Save car before mark() tramples on it
    p->mark();
    mark(q);
    p = p->cdr();
    if (p->isMarked())
      return;
  }

  // Here, *p is an unmarked atom
  p->mark();
}

// Free all unmarked cells
void Heap::sweep() {
  for (int i = 0; i < nCells; ++i) {
    if (heap[i].isMarked())
      heap[i].unmark();
    else if (heap[i].notFree())
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

Cell* Heap::alloc(StringHead* s, Tag t)
{
  return alloc()->set(s, t);
}

Cell* Heap::cons(Cell* a, Cell* d) { return alloc()->set(a, d); }

void Heap::dump()
{
    cout << "--- start heap ---" << endl;
    cout << "nCells = " << nCells << endl;
    cout << "nFree = " << nFree << endl;
    cout << "pFree = " << std::hex << (long int) pFree << endl << std::dec;
    cout << "pProtected = " << std::hex << (long int) pProtected << endl << std::dec;
    for (int i = 0; i < nCells; ++i) {
      if (heap[i].notFree())
	heap[i].dump();
    }
    cout << "--- end   heap ---" << endl;
}

void Heap::gc()
{
  mark();
  sweep();

  // TODO: Compactify string space
}

Cell* Heap::makeList(Cell* a)
{
  return cons(a, nil());
}

Cell* Heap::makeList(Cell* a, Cell* b)
{
  return cons(a, cons(b, nil()));
}

Cell* Heap::makeList(Cell* a, Cell* b, Cell* c)
{
  return cons(a, cons(b, cons(c, nil())));
}

Cell* Heap::makeString(const char* s)
{
  auto sp = strings->alloc(s);
  auto op = alloc(sp, STRING_TAG);
  sp->set(op);
  return op;
}

Cell* Heap::makeSymbol(const char* s)
{
  auto sp = strings->alloc(s);
  auto op = alloc(sp, SYMBOL_TAG);
  sp->set(op);
  return op;
}
