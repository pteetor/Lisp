//
// Unit test for HashTable object
//
#include <cassert>

#include "globals.h"
#include "Object.h"
#include "Heap.h"

using namespace std;

int main()
{
  StringSpace sSpace(1000);
  Heap heap(1000, &sSpace);
  HashTable hash(heap, 16);

  Object* hello_1 = hash.get("hello");
  assert(hello_1->eq(hello_1));

  cout << "hello_1: " << std::hex << hello_1 << endl;
  
  Object* goodbye = hash.get("goodbye");
  assert(goodbye->neq(hello_1));

  cout << "goodbye: " << std::hex << goodbye << endl;
  
  Object* hello_2 = hash.get("hello");
  assert(hello_2->eq(hello_1));

  cout << "hello_2: " << std::hex << hello_2 << endl;

  cout << "All tests completed" << endl;
  
  return 0;
}
