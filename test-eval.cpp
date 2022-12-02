//
// Unit tests for eval() function
//

#include <cassert>
#include <iostream>

using namespace std;

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "Interp.h"

int main()
{
  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);
  Interp interp(heap);

  // DEBUG
  cout << "LAMBDA = " << *S_LAMBDA << endl;

  // DEBUG
  dumpGlobalSymbols();
  dumpGlobalEnv();

  auto nil = heap.nil();

  // nil evaluates to nil
  {
    interp.eval(nil, nil);
    auto result = heap.pop();
    assert(result->null());
  }

  // Constants evaluate to themselves
  {
    heap.alloc(3.14);
    heap.push(nil);
    interp.eval();
    double result = (double) *(heap.pop());
    cout << result << endl;
    assert(result == 3.14);
  }

  {
    auto fp = heap.newFrame();
    heap.makeString("Foo");
    auto foo = heap.top();
    heap.push(nil);
    interp.eval();
    auto result = heap.popFrame(fp);
    print(result); cout << endl;
    assert(result->eq(foo));    
  }

  // Global symbols evaluate to themselves
  {
    heap.makeSymbol("pi");
    interp.eval(heap.pop());
    auto result = heap.pop();
    print(result); cout << endl;
    assert((double) *result == 3.1415926);
  }

  {
    heap.makeSymbol("+");
    interp.eval(heap.pop());
    auto result = heap.pop();
    print(result); cout << endl;
    assert(result->functionp());
  }

  cout << "All tests passed" << endl;
  return 0;
}
