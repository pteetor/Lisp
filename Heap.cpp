//
// Implementation of Heap object class
//
#include <iostream>

#include "Object.h"
#include "Heap.h"
#include "functions.h"

//
// Heap - private methods
//

Heap::Heap(int n, StringSpace *ss) {
  nObjects = n;
  heap = new Object[n];
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

Object* Heap::alloc() {
  if (pFree->eq(nil())) throw std::bad_alloc();

  // Double-check:
  if (nFree == 0) throw std::bad_alloc();

  auto p = pFree;
  pFree = pFree->cdr();

  nFree--;
  return p;
}

void Heap::free(Object* p)
{
  p->setFree();
  p->replacd(pFree);
  pFree = p;
  ++nFree;
}

void Heap::protect(Object* p)
{
  pProtected = cons(p, pProtected);
}

void Heap::unprotect(Object* p)
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
  Object* p = pProtected;
  Object* q;
  
  while (p->neq(nil())) {
    q = p->car();   // Save car before mark() tramples on it
    p->mark();
    mark(q);
    p = p->cdr();
  }
}

// Mark specific cells
void Heap::mark(Object* p)
{
  if (p->isMarked())
    return;

  Object* q;
  
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
  for (int i = 0; i < nObjects; ++i) {
    if (heap[i].isMarked())
      heap[i].unmark();
    else if (heap[i].notFree())
      free(&heap[i]);
  }
}

//
// Heap - public methods
//

Object* Heap::alloc(bool b) { return alloc()->set(b); }
Object* Heap::alloc(char c) { return alloc()->set(c); }
Object* Heap::alloc(int i) { return alloc()->set(i); }
Object* Heap::alloc(double d) { return alloc()->set(d); }

// Allocate string object
Object* Heap::alloc(String* s)
{
  return alloc()->set(s);
}

// Allocate symbol object
Object* Heap::alloc(Object* pname)
{
  return alloc()->set(pname);
}

Object* Heap::cons(Object* a, Object* d) { return alloc()->set(a, d); }

void Heap::dump()
{
    cout << "--- start heap ---" << endl;
    cout << "nObjects = " << nObjects << endl;
    cout << "nFree = " << nFree << endl;
    cout << "pFree = " << std::hex << (long int) pFree << endl << std::dec;
    cout << "pProtected = " << std::hex << (long int) pProtected << endl << std::dec;
    for (int i = 0; i < nObjects; ++i) {
      if (heap[i].notFree())
	heap[i].dump();
    }
    cout << "--- end   heap ---" << endl;
}

void Heap::gc()
{
  mark();
  // TODO: Sweep the hash table buckets, removing unmarked strings
  sweep();
  // TODO: Compactify string space, removing marks
}

Object* Heap::makeList(Object* a)
{
  return cons(a, nil());
}

Object* Heap::makeList(Object* a, Object* b)
{
  return cons(a, cons(b, nil()));
}

Object* Heap::makeList(Object* a, Object* b, Object* c)
{
  return cons(a, cons(b, cons(c, nil())));
}

Object* Heap::makeString(const char* s)
{
  auto sp = strings->alloc(s);
  auto op = alloc(sp);
  sp->set(op);
  return op;
}

Object* Heap::makeSymbol(const char* s)
{
  auto strobj = makeString(s);
  return alloc(strobj);
}
