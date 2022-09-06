//
// Lisp reader for SExpers
//

// Needs: #include "tokenizer.h"
// Needs: #include "interp.h"

class Reader {
  Tokenizer *tkz;
  CellHeap *heap;

  Cell *parseList() {
    tkz->next();   // Skip '('

    if (tkz->token() == RPAREN_TOK) {
      tkz->next();
      return nil;
    }
    
    Cell *car = parse();
    Cell *head = heap->cons(car, nil);
    Cell *tail = head;

    while (tkz->token() != DOT_TOK)
      {
	if (tkz->token() == RPAREN_TOK)
	  {
	    tkz->next();
	    return head;
	  }
	Cell *elem = parse();
	tail->replacd(heap->cons(elem, nil));
	tail = tail->cdr();
      }

    // Here tail ends with: . <expr> )
    tkz->next();   // Skip dot
    Cell *cdr = parse();
    tail->replacd(cdr);
    // TODO: token != ")"  =>  syntax error
    tkz->next();   // Skip ")"
    return head;
  }

  Cell *parse() {
    // TODO: If EOF, throw EOF condition
    
    switch(tkz->token())
      {
      case INT_TOK:
	return heap->alloc(std::atoi(tkz->tokenString()));
      case DOUBLE_TOK:
	return heap->alloc(std::atof(tkz->tokenString()));
      case STRING_TOK:
	return heap->alloc(tkz->tokenString());
      case SYMBOL_TOK:
	return heap->allocSymbol(tkz->tokenString());

      case LPAREN_TOK:
	return parseList();
      }
  }

 public:
  Reader(Tokenizer* t, CellHeap *h) {
    tkz = t;
    heap = h;
    tkz->init();
  }

  bool eof() { return tkz->token() == EOF_TOK; }
  Cell *next() { return parse(); }
};
