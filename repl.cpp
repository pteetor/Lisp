//
//  REPL loop for Lisp interpreter
//

#include <iostream>
#include <sstream>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "Tokenizer.h"
#include "Reader.h"
#include "Interp.h"

int main()
{
  // Useful for testing:
  // std::string theInput = "( (lambda (x y) (+ x (* 2 y))) 10 3.14)";
  // std::stringstream theStream(theInput);
  // Tokenizer tkz(theStream);

  Tokenizer tkz(cin);

  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);
  Reader rdr(tkz, heap);

  EchoInterp interp(heap);

  while (rdr.read())
    {
      heap.push(heap.nil());
      interp.eval();
      print(heap.top());
      cout << endl;
      heap.pop();
    }

  exit(0);
}
