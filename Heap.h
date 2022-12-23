//
// Heap management for Lisp
//

//
// Heap management
//
class Heap {
  static const int STACK_SIZE = 1000;
  
  ObjPool* obj;
  StringFinder* finder;
  Object* dict;               // List of <key,value> pairs
  Object* stack[STACK_SIZE];
  Object** sp;
  
  Object *pProtected;   // List of protected spaces

  void mark();
  void mark(Object*);
  
public:
  Heap(ObjPool* op, StringFinder* sf);
  ~Heap();

  int nFreeObjects() { return obj->nFreeObjects(); }

  Object* nil() const { return obj->nil(); }

  // Q: Should these be 'void' functions? They leave their value on the stack
  Object* alloc(bool b);
  Object* alloc(char c);
  Object* alloc(int i);
  Object* alloc(double d);
  Object* alloc(String* s);             // New string
  Object* alloc(const char* s);
  Object* alloc(NativeFunction* p);     // New built-in function
  Object* alloc(NativeMacro* m);        // New built-in macro
  
  Object* allocClosure(Object* p);
  Object* allocSymbol(Object* pname);

  void cons();                          // Implicit car & cdr on stack
  void cons(int n);                     // Perform N cons() calls
  Object* cons(Object* a, Object* d);   // Explicit car & cdr

  // Stack functions
  Object* down(int n);
  void drop(int n);
  Object* push(Object* p);
  Object* pop();

  Object* top() const;
  void replace(Object* newTop, int n = 1);

  // Frame functions
  Frame newFrame(int n) const;
  Frame newFrame(Object* arg0);
  Frame newFrame(Object* arg0, Object* arg1);
  
  void collapse(int n);
  void collapse(int n, Object* value);
  void collapse(Frame& f);
  void collapse(Frame& f, Object* value);

  // Dictionary methods
  // The dictionary maps strings to symbols
  static Object* key(Object* node);
  static Object* value(Object* node);
  static Object* next(Object* node);

  Object* find(Object* k);    // Find <key,value> pair within the dictionary; or nil
  bool lookupSymbol();        // Given <string> on stack, translate to corresponding symbol
  void insertSymbol();        // Given <string,symbol> on stack, insert into dict

  // Object protection methods - need for global symbols
  void protect(Object*);
  void unprotect(Object*);
  int nProtected();
  void gc();
  
  void dump();

  void makeString(const char*);                 // Leave result on stack
  void makeSymbol(const char*);                 // Leave result on stack

  Object* makeList(Object*);                    // Obsolete??
  Object* makeList(Object*, Object*);           // Obsolete??
  Object* makeList(Object*, Object*, Object*);  // Obsolete??
  void makeList(int n);                         // Stack-arguments version
};
