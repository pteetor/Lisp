//
// Simple Interp class
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "functions.h"
// #include "nativeFunctions.h"

#include "Frame.h"
#include "Interp.h"

//
// Interpreter symbols
//
Object* S_QUOTE;
Object* S_LAMBDA;
Object* S_LET;

//
// Global variables
//
Object* Interp::interpSymbols = NULL;
Object* Interp::globalEnv = NULL;

// ----------------------------------------------------------

Interp::Interp(Heap& h) : heap(h)
{
  interpSymbols = heap.nil();
  createSymbols();
  heap.protect(interpSymbols);

  globalEnv = emptyEnv(heap.nil());
  populateGlobalEnv();
  heap.protect(globalEnv);
}

//
// Eval() with explict expr and env
//
// Stack action: <...> -> <..., value>
//
void Interp::eval(Object* expr, Object* env)
{
  Frame f = heap.newFrame(expr, env);
  eval(f);
}

//
// eval() with implicit parameters on stack.
// Stack action: <..., expr, env> -> <..., value>
//
void Interp::eval(Frame& f) {
  Object* expr = f.arg(0);
  Object* env = f.arg(1);

  if (expr->symbolp()) {
    // TODO: Consult 'env' before global definition
    Object* pair = get(globalEnv, expr);
    if (pair->null())
      throw LispEx(A8);
    heap.collapse(f, pair->cdr());
    return;
  }

  if (expr->atom()) {
    heap.collapse(f, expr);
    return;
  }

  // Here, expr is a cons cell, representing a function application.
  // Evaluate the car() to find the function.
  Object* fn = expr->car();

  //
  // Evaluate the function until we get something callable
  //
  // Cases to handle:
  //   - symbol: dereference it
  //   - cons: evaluate it
  //   - constant: fail
  //
  // Loop invariant: Top of stack is a (possible) function
  //
  while (!fn->callablep()) {
    if (fn->symbolp()) {
      Object* pair = get(env, fn);
      if (pair->null())
	throw LispEx(A8);
      fn = pair->cdr();
    } else if (fn->consp()) {
      Frame f2 = heap.newFrame(fn, env);
      eval(f2);
      fn = heap.pop();
    } else if (fn->constantp()) {
      throw LispEx(A9);
    } else {
      throw LispEx(A9);  // Should never happen
    }
  }

  // Begin building a simple call-frame:
  //   - Function first (on the bottom)
  //   - Args vector above that

  Frame fp = heap.newFrame(3);
  heap.push(fn);
  heap.push(expr->cdr());
  heap.push(env);
  apply(fp);
  heap.collapse(f);
}

void Interp::apply(Frame& f)
{
  Object* fn = f.arg(0);
  Object* args = f.arg(1);
  Object* env = f.arg(2);
  
  if (fn->macrop()) {                        // Native macro
    Frame f2 = heap.newFrame(args);
    fn->callMacro(f2, env, heap);
    heap.collapse(f, heap.top());
  } else if (fn->functionp()) {              // Native function
    int nArgs = length(args);
    Frame f2 = heap.newFrame(nArgs);
    evalFrame(args, env);
    fn->callFunction(f2, heap);
    heap.collapse(f);
  }
  // LATER: if fn is a closure, apply it to args
  else {
    throw LispEx(A9);    // Should never happen
  }
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

// Stack action: <env, symbol, value> -> <>
void Interp::bind()
{
  Object* env = heap.down(2);
  Object* pairs = env->car();
  
  heap.cons();        // Create <symbol, value> pair

  // Prepend new pair to pairs list of env (in car of env)
  heap.push(pairs);
  heap.cons();

  // Update env's pointer to its pairs list
  env->replaca(heap.pop());
  heap.drop(1);
}

void Interp::bind(Object* env, const char* symbol, double x)
{
  heap.push(env);
  heap.makeSymbol(symbol);
  heap.alloc(x);
  bind();
}

void Interp::bind(Object* env, const char* symbol, NativeFunction* fun)
{
  heap.push(env);
  heap.makeSymbol(symbol);
  heap.alloc(fun);
  bind();
}

void Interp::bind(Object* env, Object* symbol, NativeMacro* mac)
{
  heap.push(env);
  heap.push(symbol);
  heap.alloc(mac);
  bind();
}

Object* Interp::evlis(Object* ls, Object* env)
{
  // TBD
  // PLACE HOLDER:
  return heap.nil();
}

Object* Interp::get(Object* env, Object* symbol)
{
  while (env->nonNull())
    {
      for (Object* pairs = env->car(); pairs->nonNull(); pairs = pairs->cdr()) {
	if (pairs->car()->car()->eq(symbol))
	  return pairs->car();
      }
      env = env->cdr();
    }
  return heap.nil();
}

Object* Interp::emptyEnv(Object* parent)
{
  return heap.cons(heap.nil(), parent);
}

// ----------------------------------------------------------

//
// Interpreter symbols and the global environment
//

// The interpreter needs some semantic symbols
void Interp::createSymbols()
{
  createSymbol(&S_QUOTE, "quote");
  createSymbol(&S_LAMBDA, "lambda");
  createSymbol(&S_LET, "let");
}

void Interp::createSymbol(Object** var, const char* str)
{
  heap.makeSymbol(str);
  *var = heap.top();

  // Append to list of protected symbols
  heap.push(interpSymbols);
  heap.cons();
  interpSymbols = heap.pop();
}

#if 0
/* MOVED */
void Interp::populateGlobalEnv()
{
  bind(globalEnv, S_QUOTE, quote_f);
  
  bind(globalEnv, "-", diff_f);
  bind(globalEnv, "/", div_f);
  bind(globalEnv, "*", prod_f);
  bind(globalEnv, "+", sum_f);

  bind(globalEnv, "pi", 3.1415926);

  bind(globalEnv, "car", car_f);
  bind(globalEnv, "cdr", cdr_f);
  bind(globalEnv, "cons", cons_f);
  bind(globalEnv, "list", list_f);
  bind(globalEnv, "sqrt", sqrt_f);
}
#endif

// ----------------------------------------------------------

//
// Friend functions
//
void dumpGlobalEnv()
{
  Object* pairs = Interp::globalEnv->car();

  for (Object* p = pairs; p->nonNull(); p = p->cdr())
    {
      print(p->car());
      std::cout << std::endl;
    }
}

void dumpInterpSymbols()
{
  print(Interp::interpSymbols);
  cout << endl;
}
