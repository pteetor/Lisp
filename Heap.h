//
// C++ types for Lisp data structures
//

#include "globals.h"

using namespace std;

// ----------------------------------------------------------

typedef enum {
  FREE_TAG = 0,     // Cell is on the free-list
  NIL_TAG = 2,      // Singleton nil cell
  BOOL_TAG = 4,
  CHAR_TAG = 6,
  INT_TAG = 8,
  DOUBLE_TAG = 10,
  STRING_TAG = 12,
  SYMBOL_TAG = 14
} TagValue;

const long int MAX_TAG = 31;

typedef long int Tag;

const char* tagName(Tag t);

// ----------------------------------------------------------

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
	long int int_v;              /* 8 bytes */
	double double_v;
	StringHead *strhead;         /* 8 bytes */
      };
    };
  };

  static const Tag MARK_BIT = 0x1;

  void checkTag(Tag t);
    
public:
  Cell() { tag = FREE_TAG; }

  // Question: Are these actually useful? Maybe only for testing?
  Cell(bool b) { tag = BOOL_TAG; bool_v = b; }
  Cell(char c) { tag = CHAR_TAG; char_v = c; }
  Cell(int i) { tag = INT_TAG; int_v = i; }
  Cell(double d) { tag = DOUBLE_TAG; double_v = d; }
  Cell(Cell *a, Cell *b) {
    car_p = a; cdr_p = b;
  }

  Cell* setFree() { tag = FREE_TAG; return this; }
  Cell* setNil() { tag = NIL_TAG; return this; }
  Cell* set(bool b) { tag = BOOL_TAG; bool_v = b; return this; }
  Cell* set(char c) { tag = CHAR_TAG; char_v = c; return this; }
  Cell* set(int i) { tag = INT_TAG; int_v = i; return this; }
  Cell* set(double d) { tag = DOUBLE_TAG; double_v = d; return this; }
  Cell* set(StringHead* p, Tag t);
    Cell* set(Cell *a, Cell *d);

  bool isFree() const { return tag == FREE_TAG; }
  bool notFree() const { return tag != FREE_TAG; }

  // Common Lisp predicated
  bool null() const { return tag == NIL_TAG; }
  bool symbolp() const { return tag == SYMBOL_TAG; }
  bool atom() const { return tag <= MAX_TAG; }
  bool consp() const { return tag > MAX_TAG; }
  bool listp() const { return tag == NIL_TAG || tag > MAX_TAG; }
  bool numberp() const { return tag == INT_TAG || tag == DOUBLE_TAG; }
  bool integerp() const { return tag == INT_TAG; }
  bool floatp() const { return tag == DOUBLE_TAG; }
  bool characterp() const { return tag == CHAR_TAG; }
  bool stringp() const { return tag == STRING_TAG; }

  // Predicates I created
  bool nonNull() const { return tag != NIL_TAG; }
  bool boolp() const { return tag == BOOL_TAG; }
  bool doublep() const { return tag == DOUBLE_TAG; }

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

  void mark();
  void unmark();
  bool isMarked() const  { return (tag & MARK_BIT) != 0; }
  bool notMarked() const { return (tag & MARK_BIT) == 0; }

  long int markBit() const { return tag & MARK_BIT; }
  Tag pureTag() const { return tag & ~MARK_BIT; }

  friend Cell *linkString(Cell*, StringHead*);
  friend Cell *linkSymbol(Cell*, StringHead*);

  friend Cell *makeString(const char*);
  friend Cell *makeSymbol(const char*);

  friend void printAtom(const Cell* ap, ostream& os);
  friend void print(const Cell* c, ostream& os);

  void dump();
};

extern Cell *linkString(Cell* cp, StringHead* s);
extern Cell *linkSymbol(Cell* cp, StringHead* s);
extern Cell *makeString(const char* s);
extern Cell *makeSymbol(const char* s);

extern void printAtom(const Cell *ap, ostream& os = std::cout);
extern void print(const Cell* p, ostream& os = std::cout);
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
  Cell* alloc(StringHead* s, Tag t);  // STRING_TAG or SYMBOL_TAG

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

  friend Cell* linkString(Cell*, StringHead*);
  friend Cell* linkSymbol(Cell*, StringHead*);
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
