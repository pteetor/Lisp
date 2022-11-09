//
// Unit tests for symbol handling
//
#include <cassert>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"

int main()
{
  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);

  Object* x = heap.makeSymbol("x");
  assert(x->eq(x));

  Object* y = heap.makeSymbol("y");
  assert(x->neq(y));
  assert(y->neq(x));

  Object* dup_x = heap.makeSymbol("x");
  assert(dup_x->eq(x));
  assert(x->eq(dup_x));

  cout << "All tests passed" << endl;
}
