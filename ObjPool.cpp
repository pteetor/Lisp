//
// Implementation of ObjPool object class
//
#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
// #include "Heap.h"
#include "functions.h"


//
// Constructor
//
ObjPool::ObjPool(int nObj) {
  nObjects = nObj;

  heap = new Object[nObjects];

  heap[0].setNil(); 
  pFree = nil();
  nFree = 0;

  // Careful here: Don't 'free' the nil cell
  while (--nObj > 0) {
    free(&heap[nObj]);
  }
}

ObjPool::~ObjPool() {
  delete heap;
}

Object* ObjPool::alloc() {
  if (pFree->eq(nil()))
    throw LispEx(GC1);

  // Double-check:
  if (nFree == 0)
    throw LispEx(GC1);

  Object* p = pFree;
  pFree = pFree->cdr();

  nFree--;
  return p;
}

void ObjPool::free(Object* p)
{
  p->setFree();
  p->replacd(pFree);
  pFree = p;
  ++nFree;
}

void ObjPool::mark(Object* p)
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
void ObjPool::sweep() {
  for (int i = 0; i < nObjects; ++i) {
    if (heap[i].isMarked())
      heap[i].unmark();
    else if (heap[i].notFree())
      free(&heap[i]);
  }
}

Object* ObjPool::alloc(bool b) { return alloc()->set(b); }
Object* ObjPool::alloc(char c) { return alloc()->set(c); }
Object* ObjPool::alloc(int i) { return alloc()->set(i); }
Object* ObjPool::alloc(double d) { return alloc()->set(d); }

Object* ObjPool::alloc(String* s)
{
  return alloc()->set(s);
}

Object* ObjPool::alloc(NativeFunction* f)
{
  return alloc()->set(f);
}

Object* ObjPool::alloc(NativeMacro* m)
{
  return alloc()->set(m);
}

// Allocate symbol object, with initial property list
Object* ObjPool::alloc(Object* p)
{
  return alloc()->set(p);
}

Object* ObjPool::cons(Object* a, Object* d)
{
  return alloc()->set(a, d);
}

void ObjPool::dump()
{
    cout << "--- start heap ---" << endl;
    cout << "nObjects = " << nObjects << endl;
    cout << "nFree = " << nFree << endl;
    cout << "pFree = " << std::hex << (long int) pFree << endl << std::dec;
    // cout << "pProtected = " << std::hex << (long int) pProtected << endl << std::dec;
    for (int i = 0; i < nObjects; ++i) {
      if (heap[i].notFree())
	heap[i].dump();
    }
    cout << "--- end   heap ---" << endl;
}

Object* ObjPool::makeList(Object* a)
{
  return cons(a, nil());
}

Object* ObjPool::makeList(Object* a, Object* b)
{
  return cons(a, cons(b, nil()));
}

Object* ObjPool::makeList(Object* a, Object* b, Object* c)
{
  return cons(a, cons(b, cons(c, nil())));
}
