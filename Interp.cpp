//
// Simple Interp class
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "functions.h"
#include "nativeFunctions.h"

#include "Interp.h"

Interp::Interp(Heap& h) : heap(h)
{
  globalEnv = emptyEnv(heap.nil());
  defineGlobalFunctions();
}

Object* Interp::apply(Object* fn, Object* args, Object* env)
{
  // TODO
  return heap.nil();
}

void Interp::eval()
{
  Object* expr = heap.down(1);
  Object* env = heap.down(0);
  
  if (expr->symbolp()) {
    // TODO: Consult 'env' before global definition
    Object* val = get(globalEnv, expr);
    if (val->null())
      throw std::invalid_argument("undefined variable");
    heap.push(val->cdr());
    heap.collapse(2);
    return;
  } else if (expr->atom()) {
    return;
  }

  // Here, expr is a cons cell.
  // First, evaluate function
  heap.push(expr->car());
  heap.push(env);
  eval();

  // TODO:
  //   - If function, eval args
  //   - Invoke apply() with args
}

Object* Interp::bind(Object* env, Object* symbol, Object* value)
{
  Object* pair = heap.cons(symbol, value);
  env->replaca(heap.cons(pair, env->car()));
  return env;
}

Object* Interp::bind(Object* env, const char* symbol, NativeFunction* fun)
{
  // FIX: return bind(env, heap.makeSymbol(symbol), heap.alloc(fun));
  // TEMP:
  return heap.nil();
}

void Interp::defineGlobalFunctions()
{
  bind(globalEnv, "+", sum_f);
}

Object* Interp::evlis(Object* ls, Object* env)
{
  // TBD
  // PLACE HOLDER:
  return heap.nil();
}

Object* Interp::get(Object* env, Object* symbol)
{
  Object* p;
  
  while (env->nonNull())
    {
      p = env->car();
      while (p->nonNull()) {
	if (p->car()->eq(symbol))
	  return p->car();
	p = p->cdr();
      }
      env = env->cdr();
    }
  return heap.nil();
}

Object* Interp::emptyEnv(Object* parent)
{
  return heap.cons(heap.nil(), parent);
}
