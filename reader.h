//
// Lisp reader for SExpers
//

// Needs: #include "tokenizer.h"
// Needs: #include "interp.h"

class Reader {
protected:
  Tokenizer *tkz;
  Heap *heap;

  void syntaxError(const char *msg = "syntax error");
  Cell *parseList();
  Cell *parse();

public:
  Reader(Tokenizer* t, Heap *h);
  
  bool eof();
  Cell *read();
};
