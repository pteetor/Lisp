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
  double sum = 0.0;
  double arg;

  for (int i = 0; i < nArgs; ++i)
    {
      // DEBUG
      arg = (double) *(args[i]);
      cout << i << "=" << arg << endl;
      
      sum += (double) *(args[i]);
    }

  heap.alloc(sum);
}
