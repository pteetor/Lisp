//
// Lisp reader for SExpers
//

// Needs: #include "tokenizer.h"
// Needs: #include "interp.h"

class Reader {
protected:
  Tokenizer *tkz;
  CellHeap *heap;

  void syntaxError() {
    cerr << "syntax error" << endl;
    exit(1);
  }

  Cell *parseList() {
    if (tkz->token() == RPAREN_TOK) {
      return nil;
    }
    
    Cell *car = parse();
    Cell *head = heap->cons(car, nil);
    Cell *tail = head;

    tkz->next();
    while (tkz->token() != DOT_TOK)
      {
	if (tkz->token() == RPAREN_TOK)
	  {
	    return head;
	  }
	Cell *elem = parse();
	tail->replacd(heap->cons(elem, nil));
	tail = tail->cdr();
	tkz->next();
      }

    // Here tail ends with: . <expr> )
    tkz->next();   // Skip dot
    Cell *cdr = parse();
    tail->replacd(cdr);
    if (tkz->next() != RPAREN_TOK)
      syntaxError();
    return head;
  }

  Cell *parse() {
    switch(tkz->token())
      {
      case SOF_TOK:
	syntaxError();   // Tokenizer screwed up?
      case EOF_TOK:
	syntaxError();   // Incomplete expression
      case DOT_TOK:
	syntaxError();
      case LPAREN_TOK:
	tkz->next();
	return parseList();
      case RPAREN_TOK:
	syntaxError();
      case CHAR_TOK:
	syntaxError();   // TODO - Handle this
      case INT_TOK:
	return heap->alloc(std::atoi(tkz->tokenString())); break;
      case DOUBLE_TOK:
	return heap->alloc(std::atof(tkz->tokenString())); break;
      case STRING_TOK:
	return heap->alloc(tkz->tokenString()); break;
      case SYMBOL_TOK:
	return heap->allocSymbol(tkz->tokenString()); break;

      default:
	// TODO: Throw syntax error
	return nil;
      }
  }

 public:
  Reader(Tokenizer* t, CellHeap *h) {
    tkz = t;
    heap = h;
    tkz->init();
  }

  bool eof() { return tkz->token() == EOF_TOK; }
  Cell *read() {
    if (tkz->first() != SOF_TOK)
      syntaxError();
    tkz->next();
    auto p = parse();
    if (tkz->next() != EOF_TOK)
      syntaxError();
    return p;
  }
};
