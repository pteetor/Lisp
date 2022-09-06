//
// C++ types for Lisp data structures
//

#include <stdexcept>
#include <string>
#include <iostream>
#include <unordered_set>

using namespace std;

// ----------------------------------------------------------

typedef unordered_set<string> StringPool;

extern StringPool stringPool;
extern const string* uniqueStr(const string& s);
extern const string* uniqueStr(const char *s);

// ----------------------------------------------------------

typedef enum {
  NIL_TAG = 0,
  BOOL_TAG = 4,
  CHAR_TAG = 6,
  INT_TAG = 8,
  DOUBLE_TAG = 10,
  STRING_TAG = 12,
  SYMBOL_TAG = 14
} TagValue;

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
	const string *string_p;
	Cell *plist_p;
      };
    };
  };

  static const Tag MARK_BIT = 0x1;

  void checkTag(Tag t) {
    if (this->tag != t) {
      throw std::bad_cast();
    };
  }
    
public:
  Cell() { tag = NIL_TAG; }
  Cell(bool b) { tag = BOOL_TAG; bool_v = b; }
  Cell(char c) { tag = CHAR_TAG; char_v = c; }
  Cell(int i) { tag = INT_TAG; int_v = i; }
  Cell(double d) { tag = DOUBLE_TAG; double_v = d; }
  Cell(std::string *s) { tag = STRING_TAG; string_p = s; }

  Cell(Cell *a, Cell *b) {
    car_p = a; cdr_p = b;
  }

  friend Cell *makeSymbol(Cell *cp, std::string *);

  Cell* set() { tag = NIL_TAG; return this; }
  Cell* set(bool b) { tag = BOOL_TAG; bool_v = b; return this; }
  Cell* set(char c) { tag = CHAR_TAG; char_v = c; return this; }
  Cell* set(int i) { tag = INT_TAG; int_v = i; return this; }
  Cell* set(double d) { tag = DOUBLE_TAG; double_v = d; return this; }
  Cell* set(const char *s) { tag = STRING_TAG; string_p = uniqueStr(s); return this; }
  Cell* setSymbol(const char *s) { tag = SYMBOL_TAG; string_p = uniqueStr(s); return this; }
  
  Cell* set(Cell *a, Cell *d) { car_p = a; cdr_p = d; return this; }

  bool null() const { return this == NULL || tag == NIL_TAG; }
  bool isAtomic() const { return tag <= MAX_TAG; }
  bool isCons() const { return !isAtomic(); }
  
  bool isBool() const { return tag == BOOL_TAG; }
  bool isChar() const { return tag == CHAR_TAG; }
  bool isInt() const { return tag == INT_TAG; }
  bool isDouble() const { return tag == DOUBLE_TAG; }

  bool isNumeric() const { return tag == INT_TAG || tag == DOUBLE_TAG; }

  operator int() const {
    if (this->tag == INT_TAG) return this->int_v;
    if (this->tag == DOUBLE_TAG) return (int) this->double_v;
    throw std::bad_cast();
  };

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
  void unmark() { tag = tag & !MARK_BIT; }
  bool isMarked() { return (bool) (tag & MARK_BIT); }

  friend void printAtom(const Cell* ap, ostream& os);
  friend void printSExpr(const Cell* c, ostream& os);
};

extern Cell *makeSymbol(Cell* cp, std::string *s);
extern int length(Cell* list);
extern void printAtom(const Cell *ap, ostream& os = std::cout);
extern void printSExpr(const Cell* p, ostream& os = std::cout);
extern ostream& operator<<(ostream& os, const Cell& c);

// ----------------------------------------------------------

class CellHeap {
  int nCells;
  int nFree;
  Cell *heap;
  Cell *pFree;

  void initHeap(int n) {
    nFree = n;
    Cell *p = pFree = &heap[0];
    while (--n > 0) {
      p->replacd(p + 1);
      p++;
    }
    p->replacd((Cell *) NULL);
  }
 
  Cell* alloc() {

    // DEBUG
    // cout << "alloc(), entering: pFree = " << std::hex << (long int) pFree << endl;
    
    if (pFree == (Cell *) NULL) throw std::bad_alloc();
    // DEBUG - something is wrong with pFree
    if (nFree == 0) throw std::bad_alloc();

    auto p = pFree;
    pFree = pFree->cdr();

    // DEBUG
    // cout << "alloc(), exiting: pFree = " << std::hex << (long int) pFree << endl;
    // cout << "alloc(), exiting: p = " << std::hex << (long int) p << endl;
    
    nFree--;
    return p;
  }
  
public:
  CellHeap(int n) {
    nCells = n;
    heap = new Cell[n];
    initHeap(n);
  }
  ~CellHeap() {
    delete heap;
  }

  int freesize() { return nFree; }

  Cell* nil() { return alloc()->set(); }
  Cell* alloc(bool b) { return alloc()->set(b); }
  Cell* alloc(char c) { return alloc()->set(c); }
  Cell* alloc(int i) { return alloc()->set(i); }
  Cell* alloc(double d) { return alloc()->set(d); }
  Cell* allocSymbol(const char *s) { return alloc()->setSymbol(s); }

  Cell* cons(Cell* a, Cell* d) { return alloc()->set(a, d); }

  void free(Cell* p) {
    p->replaca((Cell *) NULL);
    p->replacd(pFree);
    pFree = p;
    ++nFree;
  }

  void dump()
  {
    cout << "--- start heap ---" << endl;
    cout << "nFree = " << nFree
	 << "; pFree = " << std::hex << (long int) pFree << endl;
    for (int i = 0; i < nCells; ++i) {
      cout << std::dec << i << " (" << std::hex << (long int) &heap[i] << "): "
	   << std::hex << (long int) (heap[i].car()) << ", "
	   << std::hex << (long int) (heap[i].cdr()) << endl;
    }
    cout << "--- end   heap ---" << endl;
  }
};

//
// Global definitions for Cell and CellHeap data structures
//
extern Cell* nil;
extern CellHeap theHeap;

extern void initHeap();

extern Cell* alloc(bool b);
extern Cell* alloc(char c);
extern Cell* alloc(int i);
extern Cell* alloc(double d);

extern Cell* cons(Cell* a, Cell* d);
