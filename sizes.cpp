//
// C++ types for Lisp data structures
//

#include <iostream>

using namespace std;

typedef enum {
  EMPTY_TAG = 0,
  NIL_TAG,
  BOOL_TAG,
  CHAR_TAG,
  INT_TAG,
  DOUBLE_TAG,
  STRING_TAG,
  SYMBOL_TAG
} TagValue;

const long int MAX_TAG = 31;

typedef long int Tag;

class Cell {
public:
  union {
    struct {
      Cell *car;
      Cell *cdr;
    };
    struct {
      Tag tag;
      union {
	bool bool_v;
	char char_v;
	long int int_v;
	double double_v;
	char *string_p;
	Cell *plist_p;
      };
    };
  };
};

bool isValue(Cell &c) {
  return(c.tag <= MAX_TAG);
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

  cout << "sizeof(Cell) = " << sizeof(Cell) << endl;

  Cell consCell;

  consCell.car = new Cell;
  consCell.cdr = new Cell;

  cout << "consCell.tag = " << consCell.tag << endl;

  cout << "isValue(consCell) = " << isValue(consCell) << endl;
  cout << "isCons(consCell) = " << isCons(consCell) << endl;
}

