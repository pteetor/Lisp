//
// Unit tests for global functions
//

#include "globals.h"
#include "Heap.h"
#include "functions.h"

void test_elide()
{
  Cell* elem = alloc(3.14);

  Cell* list1 = cons(alloc(3), cons(elem, cons(alloc(1), theHeap.nil())));

  cout << "test_elide" << endl;
  cout << "length before: " << length(list1) << endl;
  list1 = elide(list1, elem);
  cout << "length after: " << length(list1) << endl;
}

int main()
{
  test_elide();
}


