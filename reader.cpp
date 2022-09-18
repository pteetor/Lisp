//
//  Classes to support Lisp reader
//
#include <iostream>

#include "interp.h"
#include "tokenizer.h"
#include "reader.h"

void Reader::syntaxError(const char *msg) {
    cerr << "reader: " << msg << endl;
    exit(1);
  }

Cell* Reader::parseList() {
    if (tkz->now() == RPAREN_TOK) {
      return nil;
    }
    
    Cell *car = parse();
    Cell *head = heap->cons(car, nil);
    Cell *tail = head;

    tkz->next();
    while (tkz->now() != DOT_TOK)
      {
	if (tkz->now() == RPAREN_TOK)
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

Cell* Reader::parse() {
    switch(tkz->now())
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
	return heap->alloc(std::atoi(tkz->text())); break;
      case DOUBLE_TOK:
	return heap->alloc(std::atof(tkz->text())); break;
      case STRING_TOK:
	return heap->alloc(tkz->text()); break;
      case SYMBOL_TOK:
	return heap->allocSymbol(tkz->text()); break;

      default:
	// TODO: Throw syntax error
	return nil;
      }
}

Reader::Reader(Tokenizer* t, Heap *h) {
  tkz = t;
  heap = h;
}

bool Reader::eof() { return tkz->now() == EOF_TOK; }

Cell* Reader::read() {
  if (tkz->first() != SOF_TOK)
    syntaxError();
  tkz->next();
  auto p = parse();
  if (tkz->next() != EOF_TOK)
    syntaxError();
  return p;
}
