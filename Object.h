//
// Low-level objects for Lisp: atoms, strings, cons'es, symbols
//

#include "globals.h"

using namespace std;

// ----------------------------------------------------------

//
//  String management for Lisp
//

//
// This header immedately preceeds the body of
// every string held in StringSpace;
// hence, the string contents immediately follow
// this header.
//
// Note: The character vector following this header
// is *not* null-delimited.
//
struct String {
  short int bMark;      // If non-zero, string is marked
  short int nChar;      // Number of chars in body
  Object* cell;         // Object which owns this String

  int init(const char* s, Object* c = NULL);
  String* set(Object* c);

  char* body() const;        // Pointer to body of string (following header)
  int nAlloc() const;        // Number of bytes alloc'ed to this string
  String* next() const;      // Next String in StringSpace

  // If this String had to hold a particular string,
  // what would be the next String?
  String* next(const char* s) const;

  void copy(String* other);

  void mark();
  void unmark();
  bool isMarked() const;

  bool equal(const char* s) const;
  String* write(std::ostream& os, int limit = -1);

  // Hash value for this string
  int hash(int mod) const;

  // Hash value for an arbitrary string
  static int hash(int mod, const char* s, int n);

  // Total number of bytes needed to allocate a particular string
  static int nRequired(const char* s);
};

// ----------------------------------------------------------

extern std::ostream& operator<<(std::ostream& os, const String& h);

class StringSpace {
  int availBytes;
  int nStrings;
  String* start;
  String* frontier;
  String* end;
  
 public:
  StringSpace(int nBytes);
  ~StringSpace();

  String* alloc(const char* s, Object* c = NULL);
  void compactify();

  void dump();
};

// ----------------------------------------------------------

typedef enum {
  FREE_TAG = 0,     // Object is on the free-list
  NIL_TAG = 2,      // Singleton nil cell
  BOOL_TAG = 4,
  CHAR_TAG = 6,
  INT_TAG = 8,
  DOUBLE_TAG = 10,
  STRING_TAG = 12,
  FUNCTION_TAG = 14,
  SYMBOL_TAG = 16
} TagValue;

const long int MAX_TAG = (int) SYMBOL_TAG;

typedef long int Tag;     /* 8 bytes */

const char* tagName(Tag t);

// ----------------------------------------------------------

// Note: LSB of 'tag' field is used as the mark bit.
class Object {
  union {
    struct {
      Object *car_p;
      Object *cdr_p;
    };
    struct {
      Tag tag;
      union {
	bool bool_v;
	char char_v;
	long int int_v;          // 8 bytes
	double double_v;         // 8 bytes
	String* pstring;         // Content of string
	Object* pname;           // Print name of symbol
	NativeFunction* pfunction;
      };
    };
  };

  static const Tag MARK_BIT = 0x1;

  void checkTag(Tag t);
    
public:
  Object() { tag = FREE_TAG; }

  // Question: Are these actually useful? Maybe only for testing?
  Object(bool b) { tag = BOOL_TAG; bool_v = b; }
  Object(char c) { tag = CHAR_TAG; char_v = c; }
  Object(int i) { tag = INT_TAG; int_v = i; }
  Object(double d) { tag = DOUBLE_TAG; double_v = d; }
  Object(Object *a, Object *b) {
    car_p = a; cdr_p = b;
  }

  Object* setFree() { tag = FREE_TAG; return this; }
  Object* setNil() { tag = NIL_TAG; return this; }
  Object* set(bool b) { tag = BOOL_TAG; bool_v = b; return this; }
  Object* set(char c) { tag = CHAR_TAG; char_v = c; return this; }
  Object* set(int i) { tag = INT_TAG; int_v = i; return this; }
  Object* set(double d) { tag = DOUBLE_TAG; double_v = d; return this; }
  Object* set(String* p);                // String with pointer into string space
  Object* set(Object* pname);            // Symbol with print-name
  Object* set(NativeFunction* f);        // Wrapper for native functions
  Object* set(Object *a, Object *d);     // Cons cell

  bool isFree() const { return tag == FREE_TAG; }
  bool notFree() const { return tag != FREE_TAG; }

  // Common Lisp predicates
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
  bool functionp() const { return tag == FUNCTION_TAG; }
  bool eq(const Object* x) { return x == this; }

  // Predicates I created
  bool nonNull() const { return tag != NIL_TAG; }
  bool boolp() const { return tag == BOOL_TAG; }
  bool charp() const { return tag == CHAR_TAG; }
  bool doublep() const { return tag == DOUBLE_TAG; }
  bool neq(const Object* x) { return x != this; }

  bool equal(const char* s) const;

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

  // Common Lisp functions
  Object* car() const { return car_p; }
  Object* cdr() const { return cdr_p; }

  Object* replaca(Object* p) { this->car_p = p; return this; }
  Object* replacd(Object* p) { this->cdr_p = p; return this; }

  void mark();
  void unmark();
  bool isMarked() const  { return (tag & MARK_BIT) != 0; }
  bool notMarked() const { return (tag & MARK_BIT) == 0; }

  long int markBit() const { return tag & MARK_BIT; }
  Tag pureTag() const { return tag & ~MARK_BIT; }

  void call(int nArgs, Object** frame, Heap& heap);

  friend void printAtom(const Object* ap, ostream& os);
  friend void print(const Object* c, ostream& os);

  void dump();
};

extern Object *makeString(const char* s);
extern Object *makeSymbol(const char* s);

extern void printAtom(const Object *ap, ostream& os = std::cout);
extern void print(const Object* p, ostream& os = std::cout);
extern ostream& operator<<(ostream& os, const Object& c);
