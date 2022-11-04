//
// StringFinder implementation
//
// PROBLEM: This index-table-of-Objects design
// does not easily accomodate sharing Strings
// between string objects and symbol objects.
//
// E.g., should find(s) return a string object
// or a symbol object? What should be in 'table'?

#include "Object.h"
#include "Heap.h"

StringFinder::StringFinder(Heap &h, int n) : heap(h)
{
  nBuckets = n;
  table = new Object*[n];
  for (int i = 0; i < n; ++i) {
    table[i] = heap.nil();
  }
}

StringFinder::~StringFinder()
{
  delete table;
}

//
// Returns an Object of type string
//
Object* StringFinder::find(const char* s)
{
  int i = String::hash(nBuckets, s, strlen(s));
  Object* list = table[i];

  while (list->nonNull()) {
    if (list->car()->equal(s))
      return list->car();
    list = list->cdr();
  }

  Object *p = heap.makeString(s);
  table[i] = heap.cons(p, table[i]);
  return p;
}

void StringFinder::sweep()
{
  // TODO: Sweep through the buckets, eliding unmarked objects
}
