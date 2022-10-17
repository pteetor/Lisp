//
// Test of Tokenizer class
//

#include <iostream>
#include <sstream>
#include <string>

#include "Tokenizer.h"

int main()
{
  std::string theString = "( \n (lambda (a b) (+ a b)) two \"dogs and cats\" \n )";
  std::stringstream strm(theString);

  std::cout << "Input is: " << theString << std::endl;
  std::cout << "Tokens are:" << std::endl;

  Tokenizer tkzr(strm);
  Token t = tkzr.now();

  while (t != EOF_TOK)
    {
      std::cout << t << " " << tkzr.text() << std::endl;
      t = tkzr.next();
    }

  return 0;
}
