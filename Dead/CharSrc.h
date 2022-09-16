//
// CharSrc definitions
//

class CharSrc {
 public:
  virtual bool first(char& ch);
  virtual bool next(char& ch);
  virtual bool now(char& ch);
};

class StringSrc: public CharSrc {
  const char* src;
  const char* p;
  
 public:
  StringSrc(const char *s);
  
  bool first(char& ch);
  bool next(char& ch);
  bool now(char& ch);
};

// Later:
//   StreamSrc
//   ConsoleSrc
