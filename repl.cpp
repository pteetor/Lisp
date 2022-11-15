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
  std::string theInput = "( (lambda (x y) (+ x (* 2 y))) 10 3.14)";
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);

  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);
  Reader rdr(tkz, heap);

  SimpleInterp interp(heap);

  Object* expr = rdr.read();
  Object* value;

  while (expr->nonNull())
    {
      value = interp.eval(expr, heap.nil());
      print(value);
      cout << endl;
      expr = rdr.read();
    }

  exit(0);
}
