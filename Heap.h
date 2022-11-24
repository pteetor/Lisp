//
// Heap management for Lisp
//

//
// Heap management
//
class Heap {
  ObjPool* obj;
  StringFinder* finder;
  Dict dict;
  
  Object *pProtected;   // List of protected spaces

  void mark();
  void mark(Object*);
  
public:
  Heap(ObjPool* op, StringFinder* sf);
  ~Heap();

  int nFreeObjects() { return obj->nFreeObjects(); }

  Object* nil() { return obj->nil(); }
  Object* alloc(bool b);
  Object* alloc(char c);
  Object* alloc(int i);
  Object* alloc(double d);
  Object* alloc(String* s);             // New string
  Object* alloc(NativeFunction* p);     // New built-in function
  Object* alloc(Object* p);             // New symbol, with prop. list

  Object* cons(Object* a, Object* d);

  void protect(Object*);
  void unprotect(Object*);
  int nProtected();
  void gc();
  
  void dump();

  Object* makeString(const char*);
  Object* makeSymbol(const char*);

  Object* makeList(Object*);
  Object* makeList(Object*, Object*);
  Object* makeList(Object*, Object*, Object*);

  Object* setprop(Object* sym, Object* ind, Object* val);

  //
  // Global constants provided by the Heap
  //
  static Object* PNAME;
  static Object* APVAL;
};
