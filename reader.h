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

    if (tkz->token() == LPAREN_TOK)
      return parseList();

    Cell *p;
    
    switch(tkz->token())
      {
      case INT_TOK:
	p = heap->alloc(std::atoi(tkz->tokenString())); break;
      case DOUBLE_TOK:
	p = heap->alloc(std::atof(tkz->tokenString())); break;
      case STRING_TOK:
	p = heap->alloc(tkz->tokenString()); break;
      case SYMBOL_TOK:
	p = heap->allocSymbol(tkz->tokenString()); break;
      default:
	// TODO: Throw syntax error
	p = nil; break;
      }

    tkz->next();
    return p;
  }

 public:
  Reader(Tokenizer* t, CellHeap *h) {
    tkz = t;
    heap = h;
    tkz->init();
  }

  bool eof() { return tkz->token() == EOF_TOK; }
  Cell *read() { return parse(); }
};
