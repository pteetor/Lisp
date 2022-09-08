//
// Lisp reader for SExpers
//

// Needs: #include "tokenizer.h"
// Needs: #include "interp.h"

class Reader {
protected:
  Tokenizer *tkz;
  CellHeap *heap;

  void syntaxError(const char *msg = "syntax error");
  Cell *parseList();
  Cell *parse();

public:
  Reader(Tokenizer* t, CellHeap *h);
  
  bool eof();
  Cell *read();
};
