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
  short int bMark;      // If non-zero, string is marked
  short int nChar;      // Number of chars in body
  Cell* cell;           // Cell which owns this String

  void init(Cell* c, const char* s);

  char* body() const;        // Pointer to body of string (following header)
  int nAlloc() const;        // Number of bytes alloc'ed to this string
  StringHead* next() const;  // Next StringHead in StringSpace

  // If this StringHead had to hold a particular string,
  // what would be the next StringHead?
  StringHead* next(const char* s) const;

  void copy(StringHead* other);

  void mark();
  void unmark();
  bool isMarked() const;

  // Total number of bytes needed to allocate a particular string
  static int nRequired(const char* s);
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
