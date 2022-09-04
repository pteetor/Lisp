//
// C++ types for Lisp data structures
//

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

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
	std::string *string_p;
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
  Cell* set(std::string *s) { tag = STRING_TAG; string_p = s; return this; }
  
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

  // friend Cell* car(const Cell& c);
  // friend Cell* cdr(const Cell& c);

  Cell* replaca(Cell* p) { this->car_p = p; return this; }
  Cell* replacd(Cell* p) { this->cdr_p = p; return this; }

  void mark() { tag = tag | MARK_BIT; }
  void unmark() { tag = tag & !MARK_BIT; }
  bool isMarked() { return (bool) (tag & MARK_BIT); }

  // friend ostream& operator<<(ostream& os, const Cell& c);

  friend void printAtom(const Cell* ap, ostream& os);
  friend void printSExpr(const Cell* c, ostream& os);
};

// Friend functions
// Cell* car(const Cell &c) { return c.car_p; }
// Cell* cdr(const Cell &c) { return c.cdr_p; }

// Friend function
Cell *makeSymbol(Cell *cp, std::string *s)
{
  cp->tag = SYMBOL_TAG;
  cp->string_p = s;
  return cp;
}

int length(Cell* list) {
  int len = 0;
  Cell *p = list;
  while (!p->null()) {
    ++len;
    p = p->cdr();
  }
  return len;
}

void printAtom(const Cell *ap, ostream& os = std::cout) {  
  switch (ap->tag) {
    case NIL_TAG:
      os << "nil";
      break;
    case BOOL_TAG:
      os << (ap->bool_v ? "*T*" : "*F*");
      break;
    case CHAR_TAG:
      os << ap->char_v;
      break;
    case INT_TAG:
      os << ap->int_v;
      break;
    case DOUBLE_TAG:
      os << ap->double_v;
      break;
    case STRING_TAG:
      os << "<string>";
      break;
    case SYMBOL_TAG:
      os << "<symbol>";
      break;
    default:
      os << "???";
      break;
  }
}

void printSExpr(const Cell* p, ostream& os = std::cout)
{
  if (p->null()) {
    os << "nil";
  } else if (p->isAtomic()) {
    printAtom(p, os);
  } else {
    os << "(";
    printSExpr(p->car(), os);
    
    p = p->cdr();
    while (p->isCons()) {
      os << " ";
      printSExpr(p->car(), os);
      p = p->cdr();
    }
    if (p->null()) {
      os << ")";
    } else {
      os << " . ";
      printSExpr(p, os);
      os << ")";
    }
  }
}

ostream& operator<<(ostream& os, const Cell& c) {
  printSExpr(&c, os);
  return os;
}

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

// ----------------------------------------------------------

//
// The interpreter's global variables
//
CellHeap theHeap(1000);

Cell* alloc(bool b) { return theHeap.alloc(b); }
Cell* alloc(char c) { return theHeap.alloc(c); }
Cell* alloc(int i) { return theHeap.alloc(i); }
Cell* alloc(double d) { return theHeap.alloc(d); }

Cell* cons(Cell* a, Cell* d) { return theHeap.cons(a, d); }

Cell* nil = theHeap.nil();

// ----------------------------------------------------------

int main() {
  cout << "sizeof(short int) = " << sizeof(short int) << endl;
  cout << "sizeof(int) = " << sizeof(int) << endl;
  cout << "sizeof(long int) = " << sizeof(long int) << endl;
  cout << "sizeof(float) = " << sizeof(float) << endl;
  cout << "sizeof(double) = " << sizeof(double) << endl;
  cout << "sizeof(void *) = " << sizeof(void *) << endl;

  cout << "sizeof(Cell) = " << sizeof(Cell) << endl;

  Cell intCell(3);

  cout << endl;
  cout << "intCell.null() = " << intCell.null() << endl;
  cout << "isAtomic(intCell) = " << intCell.isAtomic() << endl;
  cout << "isCons(intCell) = " << intCell.isCons() << endl;
  cout << "isInt(intCell) = " << intCell.isInt() << endl;
  cout << "(int) intCell = " << (int) intCell << endl;
  cout << "(double) intCell = " << (double) intCell << endl;

  Cell dblCell(3.1415);

  cout << endl;
  cout << "(int) dblCell = " << (int) dblCell << endl;
  cout << "(double) dblCell = " << (double) dblCell << endl;

  Cell consCell(new Cell, new Cell);

  cout << endl;
  cout << "consCell.null() = " << consCell.null() << endl;
  cout << "consCell.isAtomic() = " << consCell.isAtomic() << endl;
  cout << "consCell.isCons() = " << consCell.isCons() << endl;

  try {
    cout << "(int) consCell = " << (int) consCell << endl;
  }
  catch (const std::bad_cast &e) {
    cout << endl << "Caught bad cast" << endl;
  }

  Cell markedCell;
  markedCell.mark();

  cout << endl;
  cout << "intCell.isMarked() = " << intCell.isMarked() << endl;
  cout << "markedCell.isMarked() = " << markedCell.isMarked() << endl;
  
  cout << endl;
  cout << "nil.null() = " << nil->null() << endl;

  CellHeap heap(100);

  cout << endl;
  cout << "heap.freesize() = " << heap.freesize() << endl;
  auto heap3 = heap.alloc(3);
  auto heap4 = heap.alloc(4);
  cout << "heap.freesize() = " << heap.freesize() << endl;
  auto heapCons = heap.cons(heap3, heap4);
  cout << "heap.freesize() = " << heap.freesize() << endl;
  cout << "heapCons->isCons() = " << heapCons->isCons() << endl;

  CellHeap uHeap(2);
  uHeap.alloc(1);
  uHeap.alloc(2);
  try {
    uHeap.alloc(3);
  }
  catch (const std::bad_alloc &e) {
    cout << endl << "Caught heap exception" << endl;
  }

  Cell* dots = cons(cons(alloc(3), alloc(2)), alloc(1));

  cout << endl;
  cout << "dots = " << *dots << endl;

  auto xlist = cons(alloc(3), cons(alloc(2), cons(alloc(1), nil)));
  cout << "xlist = " << *(xlist) <<  endl;
  
  cout << "length(xlist) = " << length(xlist) << endl;
  cout << "length(xlist->cdr()) = " << length(xlist->cdr()) << endl;
}
