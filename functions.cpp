//
// Global functions
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "functions.h"

//
// Remove an element from a list, destructively.
// Returns pointer to modified list.
//
Object* elide(Object* list, Object* elem)
{
  if (list->null())
    return list;
  if (list->car()->eq(elem))
    return list->cdr();

  Object* prev = list;
  Object* next = list->cdr();

  while (!next->null()) {
    if (next->car()->eq(elem)) {
      prev->replacd(next->cdr());
      break;
    }
    prev = next;
    next = next->cdr();
  }
  return list;
}

//
// Length of a list
//
int length(Object* list) {
  int len = 0;
  Object *p = list;
  while (!p->null()) {
    ++len;
    p = p->cdr();
  }
  return len;
}

// NOT NEEDED?
Object* linkString(Object* c, String* s)
{
  c->set(s);
  s->set(c);
  return c;
}

// NOT NEEDED?
Object* linkSymbol(Object* c, String* s)
{
  c->set(s);
  s->set(c);
  return c;
}

//
// Search a list.
// If 'elem' is found, return tail starting at 'elem'.
// Otherwise, return nil.
//
Object* member(Object* list, Object* elem)
{
  while (list->nonNull()) {
    if (list->car() == elem)
      break;
    list = list->cdr();
  }
  return list;
}

// ----------------------------------------------------------

//
// User-visible run time functions
//

Object* sum_f(Object* args, Heap& heap)
{
  double sum = 0.0;

  while (args->nonNull())
    {
      sum += (double) *(args->car());
      args = args->cdr();
    }

  return heap.alloc(sum);
}

void defineRunTimeFunctions(Heap& heap)
{
  Object *sym = heap.makeSymbol("+");
  Object* f = heap.alloc(sum_f);
  heap.setprop(sym, Heap::APVAL, f);
}
