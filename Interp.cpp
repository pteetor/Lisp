//
// Simple Interp class
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "functions.h"
#include "nativeFunctions.h"

#include "Interp.h"

//
// Interpreter symbols
//
Object* S_LAMBDA;
Object* S_LET;

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
  createSymbols();
  heap.protect(globalSymbols);

  globalEnv = emptyEnv(heap.nil());
  populateGlobalEnv();
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
    Object* pair = get(globalEnv, expr);
    if (pair->null())
      throw std::invalid_argument("undefined variable");
    heap.push(pair->cdr());
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
  createSymbol(&S_LAMBDA, "lambda");
  createSymbol(&S_LET, "let");
}

void Interp::createSymbol(Object** var, const char* str)
{
  heap.makeSymbol(str);
  *var = heap.top();

  // Append to list of protected symbols
  heap.push(globalSymbols);
  heap.cons();
  globalSymbols = heap.pop();
}

void Interp::populateGlobalEnv()
{
  bind(globalEnv, "-", diff_f);
  bind(globalEnv, "/", div_f);
  bind(globalEnv, "*", prod_f);
  bind(globalEnv, "+", sum_f);

  bind(globalEnv, "pi", 3.1415926);
}

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

void dumpGlobalSymbols()
{
  print(Interp::globalSymbols);
  cout << endl;
}
