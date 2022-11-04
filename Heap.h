//
// Heap management for Lisp
//

//
// Heap of Objects
//
// heap[0] is reserved for nil
//
class Heap {
  int nObjects;
  int nFree;
  Object *heap;
  StringSpace* strings;
  Object *pProtected;   // List of protected spaces
  Object *pFree;        // List of free spaces

  int nBuckets;
  Object** hashTable;

  Object* alloc();
  void free(Object* p);
  void mark();
  void mark(Object*);
  void sweep();
  
public:
  Heap(int nObj, StringSpace* strings, int nBuck = 1024);
  ~Heap();

  int nFreeObjects() { return nFree; }

  Object* nil() { return &heap[0]; }
  Object* alloc(bool b);
  Object* alloc(char c);
  Object* alloc(int i);
  Object* alloc(double d);
  Object* alloc(String* s);       // New string
  Object* alloc(Object* p);       // New symbol, with prop. list

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

  friend Object* linkString(Object*, String*);
  friend Object* linkSymbol(Object*, String*);
};

//
// Global definitions for Object and Heap data structures
//

extern void initHeap();

extern Object* alloc(bool b);
extern Object* alloc(char c);
extern Object* alloc(int i);
extern Object* alloc(double d);

extern Object* cons(Object* a, Object* d);
