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
#include "Frame.h"
#include "Interp.h"

void nl()
{
  cout << endl;
}

int main()
{
  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);
  Interp interp(heap);

  // DEBUG
  cout << "QUOTE = " << *S_QUOTE << endl;
  cout << "LAMBDA = " << *S_LAMBDA << endl;

  // DEBUG
  dumpInterpSymbols();
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
    Frame f = heap.newFrame(2);
    heap.alloc(1.618);
    heap.push(nil);
    interp.eval(f);
    double result = (double) *(heap.pop());
    cout << result << endl;
    assert(result == 1.618);
  }

  {
    Frame f = heap.newFrame(2);
    heap.makeString("Foo");
    auto foo = heap.top();
    heap.push(nil);
    interp.eval(f);
    auto result = heap.pop();
    print(result); cout << endl;
    assert(result->eq(foo));    
  }

  // Global symbols evaluate to themselves
  {
    Frame f = heap.newFrame(2);
    heap.makeSymbol("pi");
    heap.push(Interp::globalEnv);
    interp.eval(f);
    auto result = heap.pop();
    print(result); cout << endl;
    assert((double) *result == 3.1415926);
  }

  {
    Frame f = heap.newFrame(2);
    heap.makeSymbol("+");
    heap.push(Interp::globalEnv);
    interp.eval(f);
    auto result = heap.pop();
    print(result); cout << endl;
    assert(result->functionp());
  }

  // Does "+" distinguish integers from doubles?
  {
    heap.makeSymbol("+");
    heap.alloc(1);
    heap.alloc(1);
    heap.makeList(3);
    interp.eval(heap.top(), Interp::globalEnv);

    print(heap.top()); nl();
    cout << "That " << (heap.top()->integerp() ? "is" : "is not")
	 << " an integer" << endl;
    heap.drop(2);
  }

  // Ditto
  {
    heap.makeSymbol("+");
    heap.alloc(1.0);
    heap.alloc(1.0);
    heap.makeList(3);
    interp.eval(heap.top(), Interp::globalEnv);

    print(heap.top()); nl();
    cout << "That " << (heap.top()->integerp() ? "is" : "is not")
	 << " an integer" << endl;
    heap.drop(2);
  }

  cout << "All tests passed" << endl;
  return 0;
}
