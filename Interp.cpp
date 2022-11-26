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
  heap.protect(globalEnv);
}

Object* Interp::apply(Object* fn, Object* args, Object* env)
{
  // TODO
  return heap.nil();
}

//
// Careful: This assumes that 'expr' and 'env' are protected,
// e.g., by being in the stack.
//
void Interp::eval(Object* expr, Object* env)
{
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
  eval(expr->car(), env);
  Object* fn = heap.top();

  // TODO: branch on function vs macro, native vs lambda

  // FOR NOW, ASSUME native function
  Object** frame = heap.newFrame();
  int nArgs = evalFrame(expr->cdr(), env);

  // Invoke native function
  fn->call(nArgs, frame, heap);

  // Remove everything except function result
  heap.collapse(nArgs + 1);
}

int Interp::evalFrame(Object* args, Object* env)
{
  int n = 0;
  while (args->nonNull())
    {
      eval(args->car(), env);
      ++n;
      args = args->cdr();
    }
  return n;
}

// Stack action: <env, symbol, value> -> <env>
void Interp::bind()
{
  Object* env = heap.down(2);
  
  heap.cons();        // Create <symbol, value> pair

  // Prepend new pair to pairs list of env (in car of env)
  heap.push(env->car());
  heap.cons();

  // Update env's pointer to its pairs list
  env->replaca(heap.pop());
}

void Interp::bind(Object* env, const char* symbol, NativeFunction* fun)
{
  heap.push(env);
  heap.makeSymbol(symbol);
  heap.alloc(fun);
  bind();
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
