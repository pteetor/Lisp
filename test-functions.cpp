//
// Unit tests for global functions
//

#include "globals.h"
#include "Object.h"
#include "Heap.h"
#include "functions.h"

void test_elide()
{
  StringSpace ss(1000);
  Heap heap(1000, &ss);
  
  Object* elem = heap.alloc(3.14);
  Object* list1 = heap.makeList(heap.alloc(3), elem, heap.alloc(1));

  cout << "test_elide" << endl;
  cout << "length before: " << length(list1) << endl;
  list1 = elide(list1, elem);
  cout << "length after: " << length(list1) << endl;
}

int main()
{
  test_elide();
}


