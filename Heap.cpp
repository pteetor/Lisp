//
// Implementation of Heap object class
//
#include <iostream>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "functions.h"

//
// Global constants defined by Heap
//
Object* Heap::PNAME = NULL;   // OBSOLETE
Object* Heap::APVAL = NULL;   // OBSOLETE

//
// Heap methods
//

Heap::Heap(ObjPool* op, StringFinder *sf) : obj(op), finder(sf)
{
  dict = obj->nil();
  sp = stack;
  pProtected = obj->nil();
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

// Mark all Objects currently in use
void Heap::mark()
{
  // Nil is implicitly protected!
  nil()->mark();

  // TODO: Mark everything on the stack

  // The protected list is implicitly protected!
  Object* p = pProtected;
  Object* q;
  
  while (p->nonNull()) {
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

Object* Heap::alloc(bool b)   { return push(obj->alloc(b)); }
Object* Heap::alloc(char c)   { return push(obj->alloc(c)); }
Object* Heap::alloc(int i)    { return push(obj->alloc(i)); }
Object* Heap::alloc(double d) { return push(obj->alloc(d)); }

Object* Heap::alloc(String* s)
{
  return push(obj->alloc(s));
}

Object* Heap::alloc(NativeFunction* p)
{
  return push(obj->alloc(p));
}

// Allocate symbol object, with initial property list
Object* Heap::alloc(Object* p)
{
  return push(obj->alloc(p));
}

// Stack-argument version
void Heap::cons()
{
  push(obj->cons(down(1), down(0)));
  collapse(2);
}

Object* Heap::cons(Object* a, Object* d)
{
  return push(obj->cons(a, d));
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

// Create new list frop top N objects
// Stack action: <e1, ..., eN> -> cons(e1, cons(e2, ..., cons(eN,nil)...))
void Heap::makeList(int n)
{
  push(nil());
  while (n-- > 0)
    cons();
}

//
// Returns an Object of type string
// Stack action: <> -> <string>
//
void Heap::makeString(const char* s)
{
  push(finder->find(s));
}

// Stack action: <> -> <symbol>
void Heap::makeSymbol(const char* s)
{
  makeString(s);
  if (lookupSymbol())
    return;

  alloc(top());
  insertSymbol();
  collapse(1);     // Remove string, retain symbol
}

// ----------------------------------------------------------

//
// Stack functions
//

void Heap::collapse(int n)
{
  Object* temp = pop();
  sp -= n;
  push(temp);
}

//
// Replace entire frame with current top of stack
//
void Heap::collapseFrame(Object** fp)
{
  Object* temp = top();
  sp = fp;
  push(temp);
}

Object* Heap::down(int n)
{
  return *(sp - n - 1);
}

void Heap::drop(int n)
{
  sp -= n;
}

Object** Heap::newFrame() const
{
  return sp;
}

Object* Heap::push(Object* p)
{
  *sp++ = p;
  return p;
}

Object* Heap::pop()
{
  return *(--sp);
}

//
// Pop stack, collapse frame, and return popped value
//
Object* Heap::popFrame(Object** fp)
{
  Object* temp = pop();
  sp = fp;
  return temp;
}

Object* Heap::top() const
{
  return *(sp - 1);
}

// ----------------------------------------------------------

//
// Dicionary methods
//
// The dictionary maps strings into symbols
//

Object* Heap::find(Object* k)
{
  Object* p = dict;
  while (p->nonNull())
    {
      if (key(p)->eq(k))
	return p->car();
      p = next(p);
    }
  return obj->nil();
}

// <string, symbol> on stack
// Insert <string, symbol> pair into dictionary
// Stack action: unchanged
void Heap::insertSymbol()
{
  cons(down(1), down(0));     // Create <string,symbol> pair
  push(dict);
  cons();                     // Create new head of dict list
  dict = pop();
}

Object* Heap::key(Object* node)
{
  return node->car()->car();
}

// Stack action:
//   - If not found: <string> -> <string>
//   - If found: <string> -> <symbol>
bool Heap::lookupSymbol()
{
  Object* p = find(top());
  if (p->null())
    return false;
  else {
    pop();
    push(p->cdr());
    return true;
  }
}

Object* Heap::next(Object* node)
{
  return node->cdr();
}

Object* Heap::value(Object* node)
{
  return node->car()->cdr();
}
