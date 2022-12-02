//
// Semantic interpreter for Lisp
//

class AbstInterp {
protected:
  Heap& heap;
  
public:
  AbstInterp(Heap& h);
  virtual ~AbstInterp();

  // Evaluation - Explicit expression and environment
  virtual void eval(Object* expr, Object* env) = 0;

  // Evaluation - Implicit <expr, env> on top of stack
  void eval();
};

class EchoInterp: public AbstInterp {

public:
  EchoInterp(Heap& h) : AbstInterp(h) { }

  void eval(Object* expr, Object*) { heap.push(expr); }
};

class Interp: public AbstInterp {

  // List of semantic symbols used by interpreter
  static Object* globalSymbols;

  // Global environment of pre-defined objects
  static Object* globalEnv;

  void createSymbol(Object**, const char*);
  void createSymbols();
  void populateGlobalEnv();

  int evalFrame(Object* args, Object* env);
  Object* evlis(Object* ls, Object* env);

  // Functions for environments
  Object* emptyEnv(Object* parent);
  void bind();
  void bind(Object* env, const char* symbol, double x);
  void bind(Object* env, const char* symbol, NativeFunction* fun);
  Object* get(Object* env, Object* symbol);

public:
  Interp(Heap& h);

  void eval(Object* expr, Object* env);
  void eval(Object* expr);
  void eval();
  
  Object* apply(Object* fn, Object* args, Object* env);

  friend void dumpGlobalEnv();
  friend void dumpGlobalSymbols();
};

// ----------------------------------------------------------

//
// Global symbols used by interpreter and unit tests
//

extern Object* S_PLUS;
extern Object* S_LAMBDA;
extern Object* S_PI;

//
// Global functions
//
extern void dumpGlobalSymbols();
extern void dumpGlobalEnv();
