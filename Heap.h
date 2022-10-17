//
// Heap management for Lisp
//

using namespace std;

// ----------------------------------------------------------

//
// Heap of cells
//
// heap[0] is reserved for nil
//
class Heap {
  int nCells;
  int nFree;
  Cell *heap;
  StringSpace* strings;
  Cell *pProtected;   // List of protected cells
  Cell *pFree;

  Cell* alloc();
  void free(Cell* p);
  void mark();
  void mark(Cell*);
  void sweep();
  
public:
  Heap(int n, StringSpace* strings);
  ~Heap();

  int nFreeCells() { return nFree; }

  Cell* nil() { return &heap[0]; }
  Cell* alloc(bool b);
  Cell* alloc(char c);
  Cell* alloc(int i);
  Cell* alloc(double d);
  Cell* alloc(String* s, Tag t);  // STRING_TAG or SYMBOL_TAG

  Cell* cons(Cell* a, Cell* d);

  void protect(Cell*);
  void unprotect(Cell*);
  int nProtected();
  void gc();
  
  void dump();

  Cell *makeString(const char*);
  Cell *makeSymbol(const char*);

  Cell* makeList(Cell*);
  Cell* makeList(Cell*, Cell*);
  Cell* makeList(Cell*, Cell*, Cell*);

  friend Cell* linkString(Cell*, String*);
  friend Cell* linkSymbol(Cell*, String*);
};

//
// Global definitions for Cell and Heap data structures
//

extern void initHeap();

extern Cell* alloc(bool b);
extern Cell* alloc(char c);
extern Cell* alloc(int i);
extern Cell* alloc(double d);

extern Cell* cons(Cell* a, Cell* d);
