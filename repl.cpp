//
//  REPL loop for Lisp interpreter
//

#include <iostream>
#include <sstream>

#include "StringSpace.h"
#include "Heap.h"
#include "tokenizer.h"
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

  Cell* expr = rdr.read();
  Cell* value;

  while (expr->nonNull())
    {
      value = interp.eval(expr);  // TODO: give global dictionary; get updated dict.
      print(expr);
      cout << endl;
      expr = rdr.read();
    }

  exit(0);
}
