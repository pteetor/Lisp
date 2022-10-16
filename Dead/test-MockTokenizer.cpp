//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include "tokenizer.h"

using namespace std;

int main() {
  Token input[] = { LPAREN_TOK, SYMBOL_TOK, INT_TOK, DOUBLE_TOK, RPAREN_TOK, EOF_TOK };

  MockTokenizer mt(input);

  mt.init();
  cout << mt.token();
  while (mt.next() != EOF_TOK)
    {
      cout << " " << mt.token();
      switch(mt.token()) {
      case INT_TOK:
      case DOUBLE_TOK:
	cout << " [" << mt.tokenString() << "]"; break;
      case STRING_TOK:
	cout << " \"" << mt.tokenString() << "\""; break;
      case SYMBOL_TOK:
	cout << " <" << mt.tokenString() << ">"; break;
      }
    }
  cout << endl;
}
