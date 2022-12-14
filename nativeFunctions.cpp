//
// User-visible run time functions
//
#include <cmath>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
// #include "Dict.h"
#include "Heap.h"
#include "functions.h"
#include "Frame.h"
#include "Interp.h"
// #include "nativeFunctions.h"

void car_f(Frame& f, Heap& heap)
{
  f.checkNArgs(1);
  Object* arg = f.arg(0);

  if (arg->atom() && arg->notFree())
    throw LispEx(X1);

  // heap.push(arg->car());
  heap.collapse(f, arg->car());
}

void cdr_f(Frame& f, Heap& heap)
{
  f.checkNArgs(1);
  Object* arg = f.arg(0);

  if (arg->atom() && arg->notFree())
    throw LispEx(X1);

  heap.collapse(f, arg->cdr());
}

void cons_f(Frame& f, Heap& heap)
{
  f.checkNArgs(2);
  heap.cons(f.arg(0), f.arg(1));
  heap.collapse(f);
}

void diff_f(Frame& f, Heap& heap)
{
  int nArgs = f.nArgs();
  
  if (nArgs == 0)
    throw LispEx(F3);

  double diff = (double) *(f.arg(0));

  if (nArgs == 1) {
    diff = -(diff);
  } else {
    for (int i = 1; i < nArgs; ++i)
      {
	diff -= (double) *(f.arg(i));
      }
  }

  heap.alloc(diff);
  heap.collapse(f);
}

void div_f(Frame& f, Heap& heap)
{
  int nArgs = f.nArgs();
  if (nArgs == 0)
    throw LispEx(F3);

  double div = (double) *(f.arg(0));

  if (nArgs == 1) {
    div = 1.0 / div;
  } else {
    for (int i = 1; i < nArgs; ++i)
      {
	div /= (double) *(f.arg(i));
      }
  }

  heap.alloc(div);
  heap.collapse(f);
}

void floatp_f(Frame& f, Heap& heap)
{
  f.checkNArgs(1);
  heap.alloc(f.arg(0)->floatp());
  heap.collapse(f);
}

void integerp_f(Frame& f, Heap& heap)
{
  f.checkNArgs(1);
  heap.alloc(f.arg(0)->integerp());
  heap.collapse(f);
}

//
// The lambda(parms,expr) creates a closure object
// from the lambda expression and the environment.
//
void lambda_f(Frame& f, Object* env, Heap& heap)
{
  f.checkMacroArgs(2);
  Object* arg = f.arg(0);

  heap.push(arg->car());
  heap.push(arg->cdr()->car());
  heap.push(env);
  heap.makeList(3);
  heap.allocClosure(heap.top());
  heap.collapse(f);
}

void list_f(Frame& f, Heap& heap)
{
  int nArgs = f.nArgs();
  
  heap.push(heap.nil());

  // Loop invariant: Top of stack is the tail of the list so far
  while (nArgs > 0) {
    heap.cons(f.arg(--nArgs), heap.top());
    heap.collapse(1);
  }

  heap.collapse(f);
}

void prod_f(Frame& f, Heap& heap)
{
  int nArgs = f.nArgs();
  if (nArgs == 0)
    throw LispEx(F3);
    
  double prod = 1.0;
  int i = 0;

  while (i < nArgs)
    {
      prod *= (double) *(f.arg(i++));
    }

  heap.alloc(prod);
  heap.collapse(f);
}

void quote_f(Frame& f, Object* env, Heap& heap)
{
  f.checkNArgs(1);
  heap.collapse(f, f.arg(0)->car());
}

void sqrt_f(Frame& f, Heap& heap)
{
  f.checkNArgs(1);
  heap.alloc(sqrt((double) *(f.arg(0))));
  heap.collapse(f);
}

void sum_f(Frame& f, Heap& heap)
{
  int nArgs = f.nArgs();
  int i_sum = 0;
  int i = 0;

  if (nArgs == 0)
    throw LispEx(F3);

  while (i < nArgs && f.arg(i)->integerp())
    {
      i_sum += (int) *(f.arg(i++));
    }

  if (i == nArgs) {
    heap.alloc(i_sum);
    return;
  }
  
  double d_sum = i_sum;

  while (i < nArgs)
    {
      d_sum += (double) *(f.arg(i++));
    }

  heap.alloc(d_sum);
}

// ----------------------------------------------------------

void Interp::populateGlobalEnv()
{
  bind(globalEnv, S_QUOTE, quote_f);
  bind(globalEnv, S_LAMBDA, lambda_f);
  
  bind(globalEnv, "-", diff_f);
  bind(globalEnv, "/", div_f);
  bind(globalEnv, "*", prod_f);
  bind(globalEnv, "+", sum_f);

  bind(globalEnv, "floatp", floatp_f);
  bind(globalEnv, "integerp", integerp_f);

  bind(globalEnv, "pi", 3.1415926);

  bind(globalEnv, "car", car_f);
  bind(globalEnv, "cdr", cdr_f);
  bind(globalEnv, "cons", cons_f);
  bind(globalEnv, "list", list_f);
  bind(globalEnv, "sqrt", sqrt_f);
}
