//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include "interp.h"
#include "CharSrc.h"
#include "tokenizer.h"
#include "reader.h"

using namespace std;

int main() {
  initHeap();

  // Token *inputs[] = {
  //   { SOF_TOK, LPAREN_TOK, RPAREN_TOK, EOF_TOK },
  //   { SOF_TOK, LPAREN_TOK, STRING_TOK, RPAREN_TOK, EOF_TOK }
  // };
    
  // Token input[] = { SOF_TOK, LPAREN_TOK, SYMBOL_TOK, RPAREN_TOK, EOF_TOK };
  // Token input[] = { SOF_TOK, LPAREN_TOK, INT_TOK, DOT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };
  // Token input[] = { SOF_TOK, LPAREN_TOK, LPAREN_TOK, RPAREN_TOK, DOT_TOK, INT_TOK, EOF_TOK };
  Token input[] = { SOF_TOK, LPAREN_TOK, SYMBOL_TOK, INT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };
  
  MockTokenizer mt(input);
  // mt.traceOn();
  
  Reader rdr(&mt, &theHeap);

  auto se = rdr.read();
  printSExpr(se);
  cout << endl;
  
  return(0);
}
