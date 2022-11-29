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

  heap.makeSymbol("x");
  Object* x = heap.top();
  assert(x->eq(x));

  cout << "print(x) = ";
  print(x);
  cout << endl;

  heap.makeSymbol("y");
  Object* y = heap.top();
  assert(y->eq(y));
  assert(x->neq(y));
  assert(y->neq(x));

  cout << "print(y) = ";
  print(y);
  cout << endl;

  heap.makeSymbol("x");
  Object* dup_x = heap.top();

  cout << "print(dup_x) = ";
  print(dup_x);
  cout << endl;

  assert(dup_x->eq(x));
  assert(x->eq(dup_x));

  cout << "All tests passed" << endl;
}
