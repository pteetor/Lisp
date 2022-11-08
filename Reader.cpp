//
//  Classes to support Lisp reader
//
#include <iostream>

#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"
#include "Tokenizer.h"
#include "Reader.h"

Reader::Reader(Tokenizer& t, Heap& h) : tkz(t), heap(h) {
  // nop
}

void Reader::syntaxError(const char *msg) {
    cerr << "reader: " << msg << endl;
    exit(1);
}

Object* Reader::parseList() {

  // Empty list?
  if (tkz.now() == RPAREN_TOK)
    return heap.nil();
    
  Object *car = parse();
  Object *head = heap.cons(car, heap.nil());
  Object *tail = head;

  tkz.next();
  while (tkz.now() != DOT_TOK)
    {
      if (tkz.now() == RPAREN_TOK)
	    return head;
      Object *elem = parse();
      tail->replacd(heap.cons(elem, heap.nil()));
      tail = tail->cdr();
      tkz.next();
    }

  // Here tail ends with: . <expr> )
  tkz.next();   // Skip dot
  Object *cdr = parse();
  tail->replacd(cdr);
  if (tkz.next() != RPAREN_TOK)
    syntaxError("missing right parenthesis");
  return head;
}

Object* Reader::parse() {
    switch(tkz.now())
      {
      case SOF_TOK:
	syntaxError("start-of-file?");   // Tokenizer screwed up?
      case EOF_TOK:
	syntaxError("unexpected end-of-input");   // Incomplete expression
      case DOT_TOK:
	syntaxError("unexpected dot");
      case LPAREN_TOK:
	tkz.next();
	return parseList();
      case RPAREN_TOK:
	syntaxError("unexpected right parenthesis");
      case CHAR_TOK:
	syntaxError();   // TODO - Handle this
      case INT_TOK:
	return heap.alloc(std::atoi(tkz.text())); break;
      case DOUBLE_TOK:
	return heap.alloc(std::atof(tkz.text())); break;
      case STRING_TOK:
	return heap.makeString(tkz.text()); break;
      case SYMBOL_TOK:
	return heap.makeSymbol(tkz.text()); break;

      default:
	syntaxError("weird token");
      }

    // Keep compiler happy
    return heap.nil();
}

bool Reader::eof() { return tkz.now() == EOF_TOK; }

Object* Reader::read() {
  if (tkz.now() == SOF_TOK)
    tkz.next();
  if (tkz.now() == EOF_TOK)
    return heap.nil();
  auto p = parse();
  if (tkz.next() != EOF_TOK)
    syntaxError("extra stuff after s-expr");
  return p;
}
