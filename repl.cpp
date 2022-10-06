//
//  REPL loop for Lisp interpreter
//

#include <iostream>
#include <sstream>

#include "Heap.h"
#include "tokenizer.h"
#include "Reader.h"
#include "Interp.h"

int main()
{
  std::string theInput = "( (lambda (x y) (+ x (* 2 y))) 10 3.14)";
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);
  Reader rdr(tkz, theHeap);

  Interp interp;

  Cell* expr = rdr.read();
  Cell* value;

  while (expr->nonNull())
    {
      value = interp.eval(expr);  // TODO: give global dictionary; get updated dict.
      printSExpr(expr);
      cout << endl;
      expr = rdr.read();
    }

  exit(0);
}
