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
#include "nativeFunctions.h"

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

  Interp interp(heap);

  while (rdr.read())
    {
      cout << "Evaluating: " << *(heap.top()) << endl;
      interp.eval(heap.top());
      print(heap.top());
      cout << endl;
      heap.drop(2);
    }

  exit(0);
}
