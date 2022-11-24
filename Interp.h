//
// Semantic interpreter for Lisp
//

class AbstInterp {
public:

  // Evaluation an expression, where
  //   e = expression
  //   a = association list or dictionary
  virtual Object* eval(Object* e, Object* a);
};

class EchoInterp {
 public:
  EchoInterp() { }

  Object* eval(Object* e, Object* a) { return e; }
};

class SimpleInterp {
  Heap& heap;
  Object* globalEnv;

  void defineGlobalFunctions();

  Object* evlis(Object* ls, Object* env);

  // Functions for environments
  Object* newEnv(Object* parent);
  Object* define(Object* env, Object* symbol, Object* value);
  Object* define(Object* env, const char* symbol, Function* fun);
  Object* get(Object* env, Object* symbol);

public:
  SimpleInterp(Heap& h);
  Object* eval(Object* e, Object* env);
  Object* apply(Object* fn, Object* args, Object* env);
};
