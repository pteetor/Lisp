//
//  REPL loop for Lisp interpreter
//

#include <iostream>
#include <sstream>

#include "Object.h"
#include "Heap.h"
#include "Tokenizer.h"
#include "Reader.h"
#include "Interp.h"

int main()
{
  std::string theInput = "( (lambda (x y) (+ x (* 2 y))) 10 3.14)";
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);

  StringSpace ss(1000);
  Heap aHeap(1000, &ss);
  Reader rdr(tkz, aHeap);

  Interp interp;

  Object* expr = rdr.read();
  Object* value;

  while (expr->nonNull())
    {
      value = interp.eval(expr);  // TODO: give global dictionary; get updated dict.
      print(expr);
      cout << endl;
      expr = rdr.read();
    }

  exit(0);
}
