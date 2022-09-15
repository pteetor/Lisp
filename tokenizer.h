//
// Classes to support Lisp reader
//

typedef enum TokenEnum {
  SOF_TOK = 0,
  EOF_TOK,
  DOT_TOK,
  LPAREN_TOK,
  RPAREN_TOK,
  CHAR_TOK,
  INT_TOK,
  DOUBLE_TOK,
  STRING_TOK,
  SYMBOL_TOK
} Token;

class Tokenizer {
protected:
  CharSrc& src;
  char ch;              // Next character to use
  bool eof;             // true when input exhausted
  Token token;
  char tokenText[256];
  char* pText;
  bool trace;
  
  Token traceToken(Token t) {
    if (trace)
      {
	std::cout << "trace: token = " << t << std::endl;
      }
    return t;
  }
  
 public:
  Tokenizer(CharSrc& s) : src(s)
  {
    token = SOF_TOK;
    eof = false;
    trace = 0;
  }
 
  virtual void init();
  virtual Token first();
  virtual Token next();
  virtual Token now();

  const char *text() { return tokenText; }
  void traceOn() { trace = true; }
  void traceOff() { trace = false; }
};

class MockTokenizer: public Tokenizer {
  int i;
  Token *tokens;
  
 public:
  MockTokenizer(Token t[]) {
    i = 0;
    tokens = t;
  }

  void init() { i = 0; }
  Token first() { i = 0; return traceToken(tokens[0]); }
  Token now() { return tokens[i]; }
  const char *tokenString()
  {
    switch (tokens[i]) {
    case INT_TOK:
      return "13";
    case DOUBLE_TOK:
      return "3.14";
    case STRING_TOK:
      return "cats and dogs";
    case SYMBOL_TOK:
      return "Foo";
    }
    return "???";   // should never happen
  }
  
  Token next() {
    return traceToken((tokens[i] == EOF_TOK) ? EOF_TOK : tokens[++i]);
  }
};

// TODO:
//   class ReplTokenizer
//   class FileTokenizer

class StringTokenizer: public Tokenizer {
  StringSrc src;
  Token token;
  char str[256];
  char *pStr;

public:
  StringTokenizer(const char* s) : src(s);

  void init() { }
  Token first();
  Token next();
  Token now();
  char *tokenString();
};
