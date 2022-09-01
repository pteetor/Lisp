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

private:
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
  
  Cell *set(bool b) { tag = BOOL_TAG; bool_v = b; return this; }
  Cell *set(char c) { tag = CHAR_TAG; char_v = c; return this; }
  Cell *set(int i) { tag = INT_TAG; int_v = i; return this; }
  Cell *set(double d) { tag = DOUBLE_TAG; double_v = d; return this; }
  Cell *set(std::string *s) { tag = STRING_TAG; string_p = s; return this; }
  
  Cell *set(Cell *a, Cell *d) { car_p = a; cdr_p = d; return this; }

  bool null() { return this == NULL || tag == NIL_TAG; }
  bool isAtomic() { return tag <= MAX_TAG; }
  bool isCons() { return !isAtomic(); }
  
  bool isBool() { return tag == BOOL_TAG; }
  bool isChar() { return tag == CHAR_TAG; }
  bool isInt() { return tag == INT_TAG; }
  bool isDouble() { return tag == DOUBLE_TAG; }

  bool isNumeric() { return tag == INT_TAG || tag == DOUBLE_TAG; }

  operator int() {
    if (this->tag == INT_TAG) return this->int_v;
    if (this->tag == DOUBLE_TAG) return (int) this->double_v;
    throw std::bad_cast();
  };

  operator double() {
    if (this->tag == DOUBLE_TAG) return this->double_v;
    if (this->tag == INT_TAG) return (double) this->int_v;
    throw std::bad_cast();
  }

  Cell *car() { return car_p; }
  Cell *cdr() { return cdr_p; }

  Cell *replaca(Cell *p) { this->car_p = p; return this; }
  Cell *replacd(Cell *p) { this->cdr_p = p; return this; }

  void mark() { tag = tag | MARK_BIT; }
  void unmark() { tag = tag & !MARK_BIT; }
  bool isMarked() { return (bool) (tag & MARK_BIT); }
};

Cell *makeSymbol(Cell *cp, std::string *s) {
  cp->tag = SYMBOL_TAG;
  cp->string_p = s;
  return cp;
}

int main() {
  cout << "sizeof(short int) = " << sizeof(short int) << endl;
  cout << "sizeof(int) = " << sizeof(int) << endl;
  cout << "sizeof(long int) = " << sizeof(long int) << endl;
  cout << "sizeof(float) = " << sizeof(float) << endl;
  cout << "sizeof(double) = " << sizeof(double) << endl;
  cout << "sizeof(void *) = " << sizeof(void *) << endl;

  cout << "sizeof(Cell) = " << sizeof(Cell) << endl;

  Cell nil;

  cout << endl;
  cout << "nil.null() = " << nil.null() << endl;
  // cout << "0->null() = " << ((Cell *) NULL)->null() << endl;

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

  auto cells = new Cell[100];

  cout << endl;
  cout << "Allocated 100" << endl;
}

