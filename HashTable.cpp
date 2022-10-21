//
// HashTable implementation
//
// PROBLEM: This index-table-of-Objects design
// does not easily accomodate sharing Strings
// between string objects and symbol objects.
//
// E.g., should get(s) return a string object
// or a symbol object? What should be in 'table'?

#include "Object.h"
#include "Heap.h"

HashTable::HashTable(Heap &h, int n) : heap(h)
{
  nBuckets = n;
  table = new Object[n];
  for (int i = 0; i < n; ++i) {
    table[i] = heap.nil();
  }
}

HashTable::~HashTable()
{
  delete table;
}

//
// Returns an anchor to a string
//
Object* HashTable::get(const char* s)
{
  int i = String::hash(nBuckets, s, strlen(s));
  Object* list = table[i];

  while (list.nonNull()) {
    if (list->car()->equal(s))
      return list->car();
    list = list->cdr();
  }

  Object *p = heap.makeAnchor(s);
  table[i] = heap.cons(p, table[i]);
  return p;
}

void HashTable::sweep()
{
  // TODO: Sweep through the buckets, eliding unmarked objects
}
