//
// Lisp reader for SExpers
//

// Needs: #include "Tokenizer.h"
// Needs: #include "Heap.h"

class Reader {
protected:
  Tokenizer& tkz;
  Heap& heap;

  void syntaxError(const char *msg = "syntax error");
  void parseList();
  void parse();

public:
  Reader(Tokenizer& t, Heap& h);
  
  bool eof();
  bool read();     // Leaves result on the stack
};
