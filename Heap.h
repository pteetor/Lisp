//
// C++ types for Lisp data structures
//

#include "globals.h"

using namespace std;

// ----------------------------------------------------------

typedef enum {
  NIL_TAG = 0,
  FREE_TAG = 2,     // Cell is on the free-list
  BOOL_TAG = 4,
  CHAR_TAG = 6,
  INT_TAG = 8,
  DOUBLE_TAG = 10,
  STRING_TAG = 12,
  SYMBOL_TAG = 14
} TagValue;

// ----------------------------------------------------------

const long int MAX_TAG = 31;

typedef long int Tag;

class Cell {
  union {
    struct {
      Cell *car_p;
      Cell *cdr_p;
    };
    struct {
      Tag tag;
      union {
	bool bool_v;
	char char_v;
	long int int_v;   /* 8 bytes */
	double double_v;
	const StringHead *strhead;
	Cell *plist_p;
      };
    };
  };

  static const Tag MARK_BIT = 0x1;

  void checkTag(Tag t);
    
public:
  Cell() { tag = NIL_TAG; }
  Cell(bool b) { tag = BOOL_TAG; bool_v = b; }
  Cell(char c) { tag = CHAR_TAG; char_v = c; }
  Cell(int i) { tag = INT_TAG; int_v = i; }
  Cell(double d) { tag = DOUBLE_TAG; double_v = d; }
  Cell(const char *s);

  Cell(Cell *a, Cell *b) {
    car_p = a; cdr_p = b;
  }

  friend Cell *makeSymbol(Cell *, const char*);

  Cell* set() { tag = NIL_TAG; return this; }
  Cell* set(bool b) { tag = BOOL_TAG; bool_v = b; return this; }
  Cell* set(char c) { tag = CHAR_TAG; char_v = c; return this; }
  Cell* set(int i) { tag = INT_TAG; int_v = i; return this; }
  Cell* set(double d) { tag = DOUBLE_TAG; double_v = d; return this; }

  Cell* set(const char *s);
  Cell* setSymbol(const char *s);
  
  Cell* set(Cell *a, Cell *d) { car_p = a; cdr_p = d; return this; }

  bool isFree() const { return tag == FREE_TAG; }
  void free() { tag = FREE_TAG; }

  bool null() const { return tag == NIL_TAG; }
  bool atom() const { return tag <= MAX_TAG; }
  bool consp() const { return !atom(); }
  bool numericp() const { return tag == INT_TAG || tag == DOUBLE_TAG; }
  
  bool isBool() const { return tag == BOOL_TAG; }
  bool isChar() const { return tag == CHAR_TAG; }
  bool isInt() const { return tag == INT_TAG; }
  bool isDouble() const { return tag == DOUBLE_TAG; }

  bool eq(const Cell* x) { return x == this; }
  bool neq(const Cell* x) { return x != this; }

  operator int() const {
    if (this->tag == INT_TAG) return this->int_v;
    if (this->tag == DOUBLE_TAG) return (int) this->double_v;
    throw std::bad_cast();
  }

  operator double() const {
    if (this->tag == DOUBLE_TAG) return this->double_v;
    if (this->tag == INT_TAG) return (double) this->int_v;
    throw std::bad_cast();
  }

  Cell* car() const { return car_p; }
  Cell* cdr() const { return cdr_p; }

  Cell* replaca(Cell* p) { this->car_p = p; return this; }
  Cell* replacd(Cell* p) { this->cdr_p = p; return this; }

  void mark() { tag = tag | MARK_BIT; }
  void unmark() { tag = tag & ~MARK_BIT; }
  bool isMarked() const { return (bool) (tag & MARK_BIT); }
  bool notMarked() const { return (bool) !(tag & MARK_BIT); }

  long int markBit() const { return tag & MARK_BIT; }
  Tag cleanTag() const { return tag & ~MARK_BIT; }

  friend void printAtom(const Cell* ap, ostream& os);
  friend void printSExpr(const Cell* c, ostream& os);

  void dump();
};

extern Cell *makeSymbol(Cell* cp, const char* s);
extern void printAtom(const Cell *ap, ostream& os = std::cout);
extern void printSExpr(const Cell* p, ostream& os = std::cout);
extern ostream& operator<<(ostream& os, const Cell& c);

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
  Cell *pProtected;   // List of protected cells
  Cell *pFree;

  Cell* alloc();
  void mark();
  void mark(Cell*);
  void sweep();
  
public:
  Heap(int n);
  ~Heap();

  int nFreeCells() { return nFree; }

  Cell* nil() { return &heap[0]; }
  Cell* alloc(bool b);
  Cell* alloc(char c);
  Cell* alloc(int i);
  Cell* alloc(double d);
  Cell* alloc(const char *s);
  Cell* allocSymbol(const char *s);

  Cell* cons(Cell* a, Cell* d);
  void free(Cell* p);

  void protect(Cell*);
  void unprotect(Cell*);
  int nProtected();
  void gc();
  
  void dump();
};

//
// Global definitions for Cell and Heap data structures
//

extern void initHeap();

extern Cell* alloc(bool b);
extern Cell* alloc(char c);
extern Cell* alloc(int i);
extern Cell* alloc(double d);
extern Cell* alloc(const char* s);

extern Cell* cons(Cell* a, Cell* d);
