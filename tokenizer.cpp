// requires: tokenizer.h

Tokenizer::init()
{
  // nop
}

void Tokenizer::lexError()
{
  std::cerr << "Lexical error" << std::endl;
  std::exit(1);
}

bool Tokenizer::nextCh()
{
  eof = src.next(ch);
  return !eof;
}

Token Tokenizer::seal(Token t)
{
  *pText = 0;
  return traceToken(t);
}

Token Tokenizer::skip(Token t)
{
  eof = src.next();
  return seal(t);
}

Token Tokenizer::scanString()
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

Token Tokenzier::scanNumeric()
{
  *(pText++) = ch;
  while (nextCh() && isdigit(ch))
    {
      *(pText++) = ch;
    }
  return seal(INT_TOK);
}

Token Tokenizer::scanSymbol()
{
  *(pText++) = ch;
  while (nextCh() && isalphanum(ch))
    {
      *pText++ = ch;
    }
  return seal(SYMBOL_TOK);
}

Token Tokenizer::scan()
{
  pText = tokenText;
  
  while (!eof) {

    // TODO: If ';', handle the comment line
    
    switch(ch) {
    case ' ':
    case '\n':
      src.next();
      continue;
    case '(':
      return skip(LPAREN_TOK);
    case ')':
      return skip(RPAREN_TOK);
    case '.':
      return skip(DOT_TOK);
    case '"':
      return scanString();
    }

    if (isdigit(ch))
      return scanNumeric();
    if (isalphnum(ch))
      return scanSymbol();
    lexError();
  }

  return traceToken(EOF_TOK);
}

Token Tokenizer::first()
{
  // TODO - init char src; scan
}

Token Tokenizer::next()
{
  // TODO - scan
}

Token Tokenizer::now()
{
  return token;
}

// ----------------------------------------------------------

StringTokenizer::StringTokenizer(const char* s) : src(s)
{
  token = SOF_TOK;
  str[0] = 0;
  pStr = str;
}

void StringTokenizer::init() { }

Token StringTokenizer::first()
{
  pStr = str;
  if (!tkz->first(str[0])) return EOF_TOK;
  return scan();
}

Token StringTokenizer::next()
{
  pStr = str;
  if (!tkz->next(str[0])) return EOF_TOK;
  return scan;
}

Token StringTokenizer::now();

char* StringTokenizer::tokenString()
{
  return str;
}
