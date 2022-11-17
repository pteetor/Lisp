//
// Test the Tokenizer when reading from console
//

#include <iostream>
#include <sstream>
#include <string>

#include "Tokenizer.h"

int main()
{
  std::cout << "Enter:" << std::endl;

  Tokenizer tkzr(std::cin);
  Token t = tkzr.now();

  while (t != EOF_TOK)
    {
      std::cout << t << " " << tkzr.text() << std::endl;
      t = tkzr.next();
    }

  std::cout << "[EOF]" << std::endl;

  return 0;
}
