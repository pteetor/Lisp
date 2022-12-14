//
//  Classes to support Lisp reader
//
#include <iostream>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
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

//
// <list>  ->  ( )
//         ->  ( <sexpr> <tail>
//
// <tail>  ->  )
//         ->  . <sexpr> )
//         ->  <sexpr> <tail> )
//
void Reader::parseList() {
  int n = 0;

  // Empty list?
  if (tkz.now() == RPAREN_TOK) {
    heap.push(heap.nil());
    return;
  }

  // Parse first element of list
  parse();
  n++;

  tkz.next();
  while (tkz.now() != DOT_TOK)
    {
      if (tkz.now() == RPAREN_TOK) {
	heap.push(heap.nil());
	heap.cons(n);
	return;
      }

      parse();
      n++;
      tkz.next();
    }

  // Here tail ends with: . <expr> )
  tkz.next();   // Skip dot
  parse();
  heap.cons(n);

  if (tkz.next() != RPAREN_TOK)
    syntaxError("missing right parenthesis");
}

void Reader::parse() {
    switch(tkz.now())
      {
      case SOF_TOK:
	syntaxError("start-of-file?");            // Tokenizer screwed up?
      case EOF_TOK:
	syntaxError("unexpected end-of-input");   // Incomplete expression
      case DOT_TOK:
	syntaxError("unexpected dot");
      case LPAREN_TOK:
	tkz.next();
	parseList();
	break;
      case RPAREN_TOK:
	syntaxError("unexpected right parenthesis");
      case CHAR_TOK:
	syntaxError();   // TODO - Handle this
      case INT_TOK:
	heap.alloc(std::atoi(tkz.text()));
	break;
      case DOUBLE_TOK:
	heap.alloc(std::atof(tkz.text()));
	break;
      case STRING_TOK:
	heap.makeString(tkz.text());
	break;
      case SYMBOL_TOK:
	heap.makeSymbol(tkz.text());
	break;

      default:
	syntaxError("weird token");
      }
}

bool Reader::eof() { return tkz.now() == EOF_TOK; }

bool Reader::read() {

  // Move to first token of the expression
  tkz.next();

  // Does that exhaust the input?
  if (tkz.now() == EOF_TOK)
    return false;
  
  parse();

  // QUESTION: How can we check that entire
  // input line (from console) is consumed
  // and no trailing crud remains?
  // (Answer: Read input via ConsoleBuffer object)

  return true;
}
