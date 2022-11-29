//
// Simple Interp class
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
// #include "Dict.h"
#include "Heap.h"
#include "functions.h"
#include "nativeFunctions.h"

#include "Interp.h"

//
// Global symbols
//
Object* S_PLUS;
Object* S_LAMBDA;

//
// Global variables
//
Object* Interp::globalSymbols = NULL;
Object* Interp::globalEnv = NULL;

// ----------------------------------------------------------

AbstInterp::AbstInterp(Heap& h) : heap(h)
{
  // nop
}

AbstInterp::~AbstInterp()
{
  // nop
}

void AbstInterp::eval()
{
  Object* expr = heap.down(1);
  Object* env = heap.down(0);
  eval(expr, env);
}

// ----------------------------------------------------------

Interp::Interp(Heap& h) : AbstInterp(h)
{
  globalSymbols = heap.nil();
  defineGlobalSymbols();
  heap.protect(globalSymbols);

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
// Eval() with implicit parameters on stack.
//
// Stack action: <expr, env> -> <expr, env, value>
//
void Interp::eval()
{
  AbstInterp::eval();
}

//
// eval() with
//   - Explicit expression, assumed to be protected
//   - Implicit environment: the global environment
//
// Stack action: <> -> <value>
//
void Interp::eval(Object* expr)
{
  eval(expr, globalEnv);
}

//
// Eval() with
//   - Explicit expression
//   - Explicit environment
//
// Careful: This assumes that 'expr' and 'env' are protected,
// e.g., by being in the stack.
//
// Stack action: <> -> <value>
//
void Interp::eval(Object* expr, Object* env)
{
  if (expr->symbolp()) {
    // TODO: Consult 'env' before global definition
    Object* val = get(globalEnv, expr);
    if (val->null())
      throw std::invalid_argument("undefined variable");
    heap.push(val->cdr());
    return;
  }

  if (expr->atom()) {
    heap.push(expr);
    return;
  }

  // Here, expr is a cons cell, representing a function application
  // Build a frame of the evaluated function andarguments
  Object** fp = heap.newFrame();
 
  // First, evaluate function
  eval(expr->car(), env);
  Object* fn = heap.top();

  // TODO: branch on function vs macro, native vs lambda

  // FOR NOW, ASSUME native function

  int nArgs = evalFrame(expr->cdr(), env);

  // Invoke native function
  fn->call(nArgs, fp + 1, heap);

  // Collapse function and args, leaving result
  heap.collapseFrame(fp);
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
  
  heap.cons();        // Create <symbol, value> pair

  // Prepend new pair to pairs list of env (in car of env)
  heap.push(env->car());
  heap.cons();

  // Update env's pointer to its pairs list
  env->replaca(heap.pop());
  heap.drop(1);
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

void Interp::defineGlobalSymbol(Object** var, const char* str)
{
  heap.makeSymbol(str);
  *var = heap.top();

  // Append to list of global symbols
  heap.push(globalSymbols);
  heap.cons();
  globalSymbols = heap.pop();
}

void Interp::defineGlobalSymbols()
{
  defineGlobalSymbol(&S_LAMBDA, "lambda");
  defineGlobalSymbol(&S_PLUS, "+");
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

// ----------------------------------------------------------

//
// Friend functions
//
void dumpGlobalSymbols()
{
  Object* p;

  for (p = Interp::globalEnv; p->nonNull(); p = p->cdr())
    {
      print(p->car()->car());
      std::cout << std::endl;
    }
}
