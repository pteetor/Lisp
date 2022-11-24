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
#include "userFunctions.h"

#include "Interp.h"

SimpleInterp::SimpleInterp(Heap& h) : heap(h)
{
  globalEnv = newEnv(heap.nil());
  defineGlobalFunctions();
}

Object* SimpleInterp::apply(Object* fn, Object* args, Object* env)
{
  // TODO
  return heap.nil();
}

Object* SimpleInterp::eval(Object* e, Object* env)
{
  if (e->symbolp()) {
    // TODO: Consult 'env' before global definition
    Object* val = get(globalEnv, e);
    if (val->null())
      throw std::invalid_argument("undefined variable");
    return val->cdr();
  } else if (e->atom()) {
    return e;
  }

  // Here, e is a cons cell
  Object* fn = eval(e->car(), env);
  // TODO: Don't eval args if fn is a macro!
  Object* args = evlis(e->cdr(), env);
  return apply(fn, args, env);
}

Object* SimpleInterp::define(Object* env, Object* symbol, Object* value)
{
  Object* pair = heap.cons(symbol, value);
  env->replaca(heap.cons(pair, env->car()));
  return env;
}

Object* SimpleInterp::define(Object* env, const char* symbol, Function* fun)
{
  return define(env, makeSymbol(symbol), heap.alloc(fun));
}

void SimpleInterp::defineGlobalFunctions()
{
  define(globalEnv, "+", sum_f);
}

Object* SimpleInterp::evlis(Object* ls, Object* env)
{
  if (ls->null())
    return ls;
  else {
    return heap.cons(eval(ls->car(), env),
		     evlis(ls->cdr(), env) );
  }
}

Object* SimpleInterp::get(Object* env, Object* symbol)
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

Object* SimpleInterp::newEnv(Object* parent)
{
  return heap.cons(heap.nil(), parent);
}
