//
//  String management for Lisp
//

class Cell;

//
// This header immedately preceeds the body of
// every string held in StringSpace;
// hence, the string contents immediately follow
// this header.
//
// TODO: This really needs a copy method
//
struct StringHead {
  Cell* cell;           // Cell which owns this String
  int nChar;            // Number of chars in body

  char* body();         // Pointer to body of string (following header)
  int nAlloc();         // Number of bytes alloc'ed to this string

  void copy(StringHead* other);
  StringHead* next();   // Next StringHead in StringSpace

  void mark();
  void unmark();
  void isMarked();

  // Total number of bytes to allocate
  static int nAlloc(char* s);
};

class StringSpace {
  int nStrings;
  StringHead* start;
  StringHead* frontier;
  void* end;
  
 public:
  StringSpace(int nBytes);

  StringHead* alloc(char* s, Cell* c);
  void compactify();
};
