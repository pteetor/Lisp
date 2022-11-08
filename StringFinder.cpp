//
// StringFinder implementation
//
// PROBLEM: This index-table-of-Objects design
// does not easily accomodate sharing Strings
// between string objects and symbol objects.
//
// E.g., should find(s) return a string object
// or a symbol object? What should be in 'table'?
//
#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"

StringFinder::StringFinder(ObjPool* op, StringSpace* ss, int n)
{
  nBuckets = n;
  obj = op;
  strings = ss;
  
  table = new Object*[n];
  for (int i = 0; i < n; ++i) {
    table[i] = obj->nil();
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

  String *sp = strings->alloc(s);
  Object *op = obj->alloc(sp);
  sp->set(op);
  
  table[i] = obj->cons(op, table[i]);
  return op;
}

void StringFinder::sweep()
{
  // TODO: Sweep through the buckets, eliding the ones
  // which contain unmarked strings
}
