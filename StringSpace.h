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
// Note: The character vector following this header
// is *not* null-delimited.
//
struct StringHead {
  Cell* cell;           // Cell which owns this String
  int nChar;            // Number of chars in body

  void init(Cell* c, const char* s);

  char* body() const;        // Pointer to body of string (following header)
  int nAlloc() const;        // Number of bytes alloc'ed to this string
  StringHead* next() const;  // Next StringHead in StringSpace

  void copy(StringHead* other);

  void mark();
  void unmark();
  bool isMarked() const;

  // Total number of bytes to allocate
  static int nAlloc(char* s);
};

extern std::ostream& operator<<(std::ostream& os, const StringHead& h);

class StringSpace {
  int availBytes;
  int nStrings;
  StringHead* start;
  StringHead* frontier;
  StringHead* end;
  
 public:
  StringSpace(int nBytes);
  ~StringSpace();

  StringHead* alloc(Cell* c, const char* s);
  void compactify();
};
