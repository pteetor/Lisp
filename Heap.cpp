//
// Implementation of Heap object class
//
#include <iostream>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "functions.h"

//
// Global constants defined by Heap
//
Object* Heap::PNAME = NULL;
Object* Heap::APVAL = NULL;

//
// Heap methods
//

Heap::Heap(ObjPool* op, StringFinder *sf) : dict(op) {
  obj = op;
  finder = sf;
  
  pProtected = obj->nil();

  // Populate global constants
  PNAME = makeString("PNAME");
  APVAL = makeString("APVAL");
}

Heap::~Heap() {
  ;   // nop for now
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

//
// Heap - public methods
//

Object* Heap::alloc(bool b) { return obj->alloc(b); }
Object* Heap::alloc(char c) { return obj->alloc(c); }
Object* Heap::alloc(int i) { return obj->alloc(i); }
Object* Heap::alloc(double d) { return obj->alloc(d); }

Object* Heap::alloc(String* s)
{
  return obj->alloc(s);
}

Object* Heap::alloc(Function* p)
{
  return obj->alloc(p);
}

// Allocate symbol object, with initial property list
Object* Heap::alloc(Object* p)
{
  return obj->alloc(p);
}

Object* Heap::cons(Object* a, Object* d)
{
  return obj->cons(a, d);
}

void Heap::dump()
{
  obj->dump();
  // TODO: dump strings, maybe dump symbol table
}

void Heap::gc()
{
  mark();
  // TODO: Sweep the hash table buckets, removing unmarked strings
  obj->sweep();
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

//
// Returns an Object of type string
//
Object* Heap::makeString(const char* s)
{
  return finder->find(s);
}

Object* Heap::makeSymbol(const char* s)
{
  Object* pname = makeString(s);
  Object* symbol = dict.lookup(pname);
  if (symbol->null()) {
    // create minimal property list with PNAME
    Object* plist = makeList(cons(PNAME, pname));
    symbol = alloc(plist);

    // Link symbol name to symbol object
    dict.insert(pname, symbol);
  }
  return symbol;
}

Object* Heap::setprop(Object* sym, Object* ind, Object* val)
{
  return obj->setprop(sym, ind, val);
}
