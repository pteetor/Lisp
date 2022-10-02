//
// Test StringSpace object
//

#include <iostream>

#include "Heap.h"
#include "StringSpace.h"

using namespace std;

int main()
{
  StringSpace space(100);

  Cell c;
  const char* s = "hello world";

  StringHead* head = space.alloc(&c, s);

  std::cout << "nChar = " << head->nChar << endl;
  std::cout << *head << endl;

  return 0;
}
