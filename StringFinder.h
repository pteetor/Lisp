//
// StingFinder classs is responsible for defining
// singleton Strings and, hence, eliminating duplicates.
//
class StringFinder {
  ObjPool* obj;
  StringSpace* strings;
  int nBuckets;
  Object** table;

 public:
  StringFinder(ObjPool*, StringSpace*, int n = 1024);
  ~StringFinder();
  
  Object* find(const char* s);
  void sweep();
};
