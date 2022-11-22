//
// ConsoleBuffer implementation
//

#include <unistd.h>   // For isatty()
#include <iostream>
#include "ConsoleBuffer.h"

using namespace std;

ConsoleBuffer::ConsoleBuffer()
{
  pBuf = buffer;
  nChar = 0;
  bEOF = false;

  bInteractive = (isatty(0) == 1) && (isatty(1) == 1);
}

char ConsoleBuffer::ahead()
{
  if (nChar > 0)
    return *(pBuf + 1);
  else
    return '\0';
}

bool ConsoleBuffer::eof() const
{
  return (nChar == 0) && bEOF;
}

bool ConsoleBuffer::more()
{
  char* q = pBuf;
  int n = nChar;

  while (n-- > 0)
    {
      ++q;
      if (*q != ' ' && *q != '\n')  // TODO: Check *all* white space
	return true;
    }
  return false;
}

char ConsoleBuffer::next()
{
  // Discard the current character, if any
  if (nChar > 0) {
    --nChar;
    ++pBuf;
  }

  // Refill as necessary
  while (nChar == 0) {
    if (!refill())
      return '\0';
  }

  return *pBuf;
}

char ConsoleBuffer::now()
{
  if (nChar > 0)
    return *pBuf;
  else
    return '\0';
}

void ConsoleBuffer::prompt()
{
  if (bInteractive)
    cout << "> ";  
}

//
// Refill our line buffer until
//   - Newline (which is saved in buffer), or
//   - Buffer is full, or
//   - End-of-file
//
bool ConsoleBuffer::refill()
{
  pBuf = buffer;
  nChar = 0;
  
  if (bEOF)
    return false;
 
  prompt();

  char* p = buffer;  
  char ch;

  for (int i = 0; i < MAX_CHARS; ++i) {
    cin.get(ch);
    if (cin.eof()) {
      bEOF = true;
      break;
    }
    *p++ = ch;
    nChar++;
    if (ch == '\n')
      break;
  }

  return !bEOF;
}
