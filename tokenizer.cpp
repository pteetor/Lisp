//
//  Tokenizer class
//

#include <istream>
#include "tokenizer.h"

Tokenizer(std::istream& s) : strm(s)
{
  token = SOF_TOK;
  eof = false;
  trace = 0;
}

void Tokenizer::lexError()
{
  std::cerr << "Lexical error" << std::endl;
  std::exit(1);
}

Token Tokenizer::traceToken(Token t) {
  if (trace) {
      std::cout << "trace: token = " << t << std::endl;
  }
  return t;
}

bool Tokenizer::nextCh()
{
  strm.get(ch);
  eof = strm.eof();
  return !eof;
}

Token Tokenizer::seal(Token t)
{
  *pText = 0;
  return traceToken(t);
}

Token Tokenizer::skip(Token t)
{
  nextCh();
  return seal(t);
}

bool Tokenizer::isDelim()
{
  switch (ch) {
  case ' ':
  case '\n':
  case '\t':
  case '(':
  case ')':
  case '"':
  case ';':
    return true;
  default:
    return false;
  }
}

Token Tokenizer::scanLiteral()
{
  while (nextCh() && ch != '"')
    {
      *(pText++) = ch;
    }
  if (ch != '"')
    lexError();
  *pText = 0;
  return skip(STRING_TOK);
}

Token Tokenizer::scanAtom()
{
  *(pText++) = ch;
  while (nextCh() && !isDelim())
    {
      *pText++ = ch;
    }
  return seal(SYMBOL_TOK);
}

Token Tokenizer::scan()
{
  pText = tokenText;
  
  while (!eof) {

    // TODO: If ';', flushLine(); continue;
    
    switch(ch) {
    case ' ':
    case '\n':
    case '\t':
      nextCh();
      continue;
    case '(':
      return skip(LPAREN_TOK);
    case ')':
      return skip(RPAREN_TOK);
    case '.':
      return skip(DOT_TOK);
    case '"':
      return scanLiteral();
    }

    return scanAtom();
  }

  return traceToken(EOF_TOK);
}

Token Tokenizer::first()
{
  nextCh();
  return scan();
}

Token Tokenizer::next()
{
  return scan();
}

Token Tokenizer::now()
{
  return token;
}

const char* Tokenizer::text()
{
  return tokenText;
}

