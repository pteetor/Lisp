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
 public:
  Tokenizer() { };
  virtual void init() { };
  virtual Token first() { return EOF_TOK; }
  virtual Token next() { return EOF_TOK; }
  virtual Token token() { return EOF_TOK; }
  virtual const char *tokenString() { return ""; }
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
  Token token() { return tokens[i]; }
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
    (tokens[i] == EOF_TOK) ? EOF_TOK : tokens[++i];
  }
};

// TODO:
//   class ReplTokenizer
//   class FileTokenizer
