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

void sum_f(int nArgs, Object** frame, Heap& heap)
{
  double sum = 0.0;

  for (int i = 0; i < nArgs; ++i)
    {
      sum += (double) *(frame[i]);
    }

  heap.alloc(sum);
}
