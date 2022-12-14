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
