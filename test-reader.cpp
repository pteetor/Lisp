//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include <sstream>
#include "Heap.h"
#include "tokenizer.h"
#include "Reader.h"

using namespace std;

int main() {

  // Token *inputs[] = {
  //   { SOF_TOK, LPAREN_TOK, RPAREN_TOK, EOF_TOK },
  //   { SOF_TOK, LPAREN_TOK, STRING_TOK, RPAREN_TOK, EOF_TOK }
  // };
    
  // Token input[] = { SOF_TOK, LPAREN_TOK, SYMBOL_TOK, RPAREN_TOK, EOF_TOK };
  // Token input[] = { SOF_TOK, LPAREN_TOK, INT_TOK, DOT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };
  // Token input[] = { SOF_TOK, LPAREN_TOK, LPAREN_TOK, RPAREN_TOK, DOT_TOK, INT_TOK, EOF_TOK };
  // Token input[] = { SOF_TOK, LPAREN_TOK, SYMBOL_TOK, INT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };
  
  // MockTokenizer mt(input);
  // mt.traceOn();

  std::string theInput = "( (lambda (x pwr) (+ x (** 2 pwr))) 10 3.14)";

  std::cout << "Input is: " << theInput << std::endl;
  
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);
  Reader rdr(tkz, theHeap);

  tkz.traceOn();
  auto se = rdr.read();

  cout << endl;
  theHeap.dump();
  
  printSExpr(se);
  cout << endl;

  
  return(0);
}
