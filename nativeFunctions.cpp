//
// User-visible run time functions
//
#include <cmath>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "functions.h"
#include "nativeFunctions.h"

void diff_f(int nArgs, Object** args, Heap& heap)
{
  if (nArgs == 0)
    throw LispEx(F3);

  double diff = (double) *(args[0]);

  if (nArgs == 1) {
    diff = -(diff);
  } else {
    for (int i = 1; i < nArgs; ++i)
      {
	diff -= (double) *(args[i]);
      }
  }

  heap.alloc(diff);
}

void div_f(int nArgs, Object** args, Heap& heap)
{
  if (nArgs == 0)
    throw LispEx(F3);

  double div = (double) *(args[0]);

  if (nArgs == 1) {
    div = 1.0 / div;
  } else {
    for (int i = 1; i < nArgs; ++i)
      {
	div /= (double) *(args[i]);
      }
  }

  heap.alloc(div);
}

void prod_f(int nArgs, Object** args, Heap& heap)
{
  double prod = 1.0;
  int i = 0;

  while (i < nArgs)
    {
      prod *= (double) *(args[i++]);
    }

  heap.alloc(prod);
}

void sqrt_f(int nArgs, Object** args, Heap& heap)
{
  if (nArgs < 1)
    throw LispEx(F3);
  if (nArgs > 1)
    throw LispEx(F2);

  heap.alloc(sqrt((double) *(args[0])));
}

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
