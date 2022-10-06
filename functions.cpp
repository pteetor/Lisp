//
// Global functions
//

#include "globals.h"
#include "Heap.h"
#include "functions.h"

//
// Length of a list
//
int length(Cell* list) {
  int len = 0;
  Cell *p = list;
  while (!p->null()) {
    ++len;
    p = p->cdr();
  }
  return len;
}

//
// Remove an element from a list, destructively.
// Returns pointer to modified list.
//
Cell* elide(Cell* list, Cell* elem)
{
  if (list->null())
    return list;
  if (list->car()->eq(elem))
    return list->cdr();

  Cell* prev = list;
  Cell* next = list->cdr();

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
