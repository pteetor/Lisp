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

#include "Interp.h"

SimpleInterp::SimpleInterp(Heap& h) : heap(h)
{
  defineRunTimeFunctions(heap);
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
    Object* val = e->get(Heap::APVAL);
    if (val->null())
      throw std::invalid_argument("undefined variable");
    return val;
  } else if (e->atom()) {
    return e;
  } else {   // cons case
    Object* fn = eval(e->car(), env);
    // TODO: Don't eval args if fn is a macro!
    Object* args = evlis(e->cdr(), env);
    return apply(fn, args, env);
  }

  // Just to keep C++ compiler happy
  heap.nil();
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
