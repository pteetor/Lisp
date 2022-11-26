//
// Semantic interpreter for Lisp
//

class AbstInterp {
public:

  // Evaluation an expression, where stack is <expr, env>
  virtual void eval();
};

class EchoInterp {
  Heap& heap;
  
 public:
  EchoInterp(Heap& h) : heap(h) { }

  void eval() { return; }
};

class Interp {
  Heap& heap;
  Object* globalEnv;

  void defineGlobalFunctions();

  Object* evlis(Object* ls, Object* env);

  // Functions for environments
  Object* emptyEnv(Object* parent);
  Object* bind(Object* env, Object* symbol, Object* value);
  Object* bind(Object* env, const char* symbol, NativeFunction* fun);
  Object* get(Object* env, Object* symbol);

public:
  Interp(Heap& h);
  void eval();
  
  Object* apply(Object* fn, Object* args, Object* env);
};
