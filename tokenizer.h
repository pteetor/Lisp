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

class AbstTokenizer {
public:
  virtual Token next() = 0;
  virtual Token now() = 0;
  virtual const char *text() = 0;
};

class Tokenizer: public AbstTokenizer {
protected:
  std::istream& strm;
  char ch;              // Next character to use
  bool eof;             // true when input exhausted
  Token token;          // The most-recently scanned token
  bool trace;           // If true, trace returned tokens
  char* pText;          // Points into tokenText
  char tokenText[256];  // The text of that token

  void lexError();
  Token traceToken(Token);
  bool nextCh();
  Token seal(Token);
  Token singleChar(Token);
  bool isDelim();
  Token scanLiteral();
  Token scanAtom();
  Token scan();

 public:
  Tokenizer(std::istream& s);

  // Required by AbstTokenizer base class
  Token next();
  Token now();
  const char *text();
  
  void traceOn() { trace = true; }
  void traceOff() { trace = false; }
};

// TODO:
//   class ReplTokenizer
//   class FileTokenizer
