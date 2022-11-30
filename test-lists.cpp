//
// Unit tests for list-related stuff
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "functions.h"

int main()
{
  ObjPool obj(1000);
  StringSpace ss(1000);
  StringFinder sf(&obj, &ss);
  Heap heap(&obj, &sf);

  Object* nil = heap.nil();

  heap.alloc("do");
  heap.alloc("re");
  heap.alloc("mi");
  heap.makeList(3);

  cout << "Should print 3:" << endl;
  cout << length(heap.top()) << endl;

  cout << "Should print (do re me):" << endl;
  print(heap.pop());
  cout << endl;

  return 0;
}
