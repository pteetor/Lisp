//
//  Tokenizer class
//

#include <iostream>
#include <regex>
#include "Tokenizer.h"

Tokenizer::Tokenizer(std::istream& s) : strm(s)
{
  ch = 0;
  eof = false;
  token = SOF_TOK;
  trace = false;
  pText = tokenText;
  tokenText[0] = 0;
}

void Tokenizer::lexError()
{
  std::cerr << "Lexical error" << std::endl;
  std::exit(1);
}

Token Tokenizer::traceToken(Token t) {
  token = t;
  if (trace) {
      std::cout << "trace: token = " << t
		<< " <" << tokenText << ">" << std::endl;
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

Token Tokenizer::singleChar(Token t)
{
  *pText++ = ch;
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
  nextCh();
  return seal(STRING_TOK);
}

Token Tokenizer::scanAtom()
{
  *(pText++) = ch;
  while (nextCh() && !isDelim())
    {
      *pText++ = ch;
    };
  *pText++ = 0;

  // TODO: Compile this regex once only
  auto doublePat = std::regex("[+-]?([0-9]+\\.)|(\\.[0-9]+)|([0-9]+\\.[0-9]+)");

  if (regex_match(tokenText, doublePat))
    return traceToken(DOUBLE_TOK);

  // TODO: Compile this regex once only
  auto integerPat = std::regex("[+-]?[0-9][0-9]*");

  if (regex_match(tokenText, integerPat))
    return traceToken(INT_TOK);
      
  return traceToken(SYMBOL_TOK);
}

Token Tokenizer::scan()
{
  pText = tokenText;
  
  while (!eof) {

    // TODO: If ';', flushLine(); continue;
    
    switch(ch) {
    case '\0':     // Initial value of 'ch'
    case ' ':
    case '\n':
    case '\t':
      nextCh();
      continue;
    case '(':
      return singleChar(LPAREN_TOK);
    case ')':
      return singleChar(RPAREN_TOK);
    case '.':
      return singleChar(DOT_TOK);
    case '"':
      return scanLiteral();
    }

    return scanAtom();
  }

  return seal(EOF_TOK);
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

