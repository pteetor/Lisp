//
// Object pool - Space management for objects
//

//
// Object pool
//
// Note: heap[0] is reserved for nil
//
class ObjPool {
  int nObjects;
  int nFree;
  Object* heap;
  Object* pFree;

  Object* alloc();
  void free(Object* p);

public:
  ObjPool(int nObj);
  ~ObjPool();

  int nFreeObjects() { return nFree; }

  void mark();
  void mark(Object*);
  void sweep();

  Object* nil() { return &heap[0]; }
  Object* alloc(bool b);
  Object* alloc(char c);
  Object* alloc(int i);
  Object* alloc(double d);
  Object* alloc(String* s);       // New string
  Object* alloc(Object* p);       // New symbol, with prop. list

  Object* cons(Object* a, Object* d);

  void dump();

  Object* makeList(Object*);
  Object* makeList(Object*, Object*);
  Object* makeList(Object*, Object*, Object*);
};
