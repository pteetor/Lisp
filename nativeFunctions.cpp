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
#include "userFunctions.h"

Object* sum_f(Object* args, Heap& heap)
{
  double sum = 0.0;

  while (args->nonNull())
    {
      sum += (double) *(args->car());
      args = args->cdr();
    }

  return heap.alloc(sum);
}
