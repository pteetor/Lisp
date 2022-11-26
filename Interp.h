//
// Semantic interpreter for Lisp
//

class AbstInterp {
public:

  // Evaluation an expression
  virtual void eval(Object* expr, Object* env);
};

class EchoInterp {
  Heap& heap;
  
 public:
  EchoInterp(Heap& h) : heap(h) { }

  void eval(Object* expr, Object*) { heap.push(expr);; }
};

class Interp {
  Heap& heap;
  Object* globalEnv;

  void defineGlobalFunctions();

  int evalFrame(Object* args, Object* env);
  Object* evlis(Object* ls, Object* env);

  // Functions for environments
  Object* emptyEnv(Object* parent);
  void bind();
  void bind(Object* env, const char* symbol, NativeFunction* fun);
  Object* get(Object* env, Object* symbol);

public:
  Interp(Heap& h);
  void eval(Object* expr, Object* env);
  
  Object* apply(Object* fn, Object* args, Object* env);
};
