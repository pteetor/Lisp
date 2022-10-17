//
// Test StringSpace object
//

#include <iostream>

#include "Object.h"
#include "Heap.h"

using namespace std;

int main()
{
  StringSpace space(100);

  const char* s = "hello world";
  StringHead* hd1 = space.alloc(s);
  std::cout << "nChar = " << hd1->nChar << endl;
  std::cout << "nAlloc = " << hd1->nAlloc() << endl;
  std::cout << *hd1 << endl;
  space.dump();

  const char* s2 = "test string";
  StringHead* hd2 = space.alloc(s2);
  std::cout << "nChar = " << hd2->nChar << endl;
  std::cout << "nAlloc = " << hd2->nAlloc() << endl;
  std::cout << *hd2 << endl;
  space.dump();

  return 0;
}
