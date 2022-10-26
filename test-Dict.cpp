//
// Unit test for Dict object class
//

#include "globals.h"
#include "Object.h"
#include "Heap.h"
#include "Dict.h"

int main()
{
  StringSpace strings(100);
  Heap theHeap(100, &strings);
  Dict dict(theHeap);

  // TBD
}
