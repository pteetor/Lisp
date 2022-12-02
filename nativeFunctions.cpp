//
// User-visible run time functions
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "functions.h"
#include "nativeFunctions.h"

void sum_f(int nArgs, Object** args, Heap& heap)
{
  int i_sum = 0;
  int i = 0;

  while (i < nArgs && args[i]->integerp())
    {
      i_sum += (int) *(args[i++]);
    }

  if (i == nArgs) {
    heap.alloc(i_sum);
    return;
  }
  
  double d_sum = i_sum;

  while (i < nArgs)
    {
      d_sum += (double) *(args[i++]);
    }

  heap.alloc(d_sum);
}
