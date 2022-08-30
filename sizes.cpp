//
// C++ types for Lisp data structures
//

#include <iostream>

using namespace std;

class Cons {
public:
  Cons *car;
  Cons *cdr;
};

const long int NIL_TAG = 0;
const long int BOOL_TAG = 1;
const long int CHAR_TAG = 2;
const long int INT_TAG = 3;
const long int DOUBLE_TAG = 4;
const long int STRING_TAG = 5;
const long int ATOM_TAG = 6;

const long int MAX_TAG = 31;

typedef long int Tag;

class Value {
public:
  Tag tag;
  union {
    bool bool_v;
    char char_v;
    long int int_v;
    double double_v;
    char *string_p;
    Cons *plist_p;
  };
};

typedef union {
  Cons cons;
  Value value;
} Cell;

bool isValue(Cell &c) {
  return(c.value.tag <= MAX_TAG);
};

bool isCons(Cell &c) {
  return(!isValue(c));
};

int main() {
  cout << "sizeof(short int) = " << sizeof(short int)<< "\n";
  cout << "sizeof(int) = " << sizeof(int)<< "\n";
  cout << "sizeof(long int) = " << sizeof(long int)<< "\n";
  cout << "sizeof(float) = " << sizeof(float) << endl;
  cout << "sizeof(double) = " << sizeof(double) << endl;
  cout << "sizeof(void *) = " << sizeof(void *) << endl;

  cout << "sizeof(Cons) = " << sizeof(Cons) << endl;
  cout << "sizeof(Value) = " << sizeof(Value) << endl;
  cout << "sizeof(Cell) = " << sizeof(Cell) << endl;

  Cell consCell;

  consCell.cons.car = new Cons;
  consCell.cons.cdr = new Cons;

  cout << "consCell.value.tag = " << consCell.value.tag << endl;

  cout << "isValue(consCell) = " << isValue(consCell) << endl;
  cout << "isCons(consCell) = " << isCons(consCell) << endl;
}

