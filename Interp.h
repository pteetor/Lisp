//
// Semantic interpreter for Lisp
//

class EchoInterp {
  Heap& heap;

public:
  EchoInterp(Heap& h) : heap(h) { }

  void eval();
  void eval(Object* expr, Object*) { heap.push(expr); }
};

class Interp {
  Heap& heap;

  // List of semantic symbols used by interpreter
  static Object* interpSymbols;

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
  void bind(Object* env, Object* symbol, NativeMacro* mac);
  Object* get(Object* env, Object* symbol);

public:
  Interp(Heap& h);

  // eval() with implicit arguments on the stack.
  // Stack action: <..., expr, env> -> <...., value>
  void eval(Frame& h);

  // eval() with explicit arguments, which need not be protected.
  // Stack action: <...> -> <..., value>
  void eval(Object* expr, Object* env);

  // Given a stack frame with three arguments:
  //   - Function
  //   - List of unevaluated arguments
  //   - Environment
  // Apply the function to the arguments
  void apply(Frame& f);

  // Global environment of pre-defined objects
  static Object* globalEnv;
  
  friend void dumpGlobalEnv();
  friend void dumpInterpSymbols();
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
