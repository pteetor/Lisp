//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include "interp.h"
#include "tokenizer.h"
#include "reader.h"

using namespace std;

int main() {
  initHeap();
  
  Token input[] = { LPAREN_TOK, SYMBOL_TOK, INT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };
  MockTokenizer mt(input);
  
  Reader rdr(&mt, &theHeap);
}
