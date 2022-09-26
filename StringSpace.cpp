//
//  Implementation of StringSpace
//

#include "StringSpace.h"

//
//  StringHead object class
//

StringHead::StringHead(Cell* c, char* s)
{
  cell = c;
  nChar = strlen(s);
}

char* StringHead::body()
{
  return (char* this) + sizeof(StringHead);
}

int StringHead::nAlloc(char* s)
{
  return sizeof(StringHead) + 4*(strlen(s) / 4);
}

void StringHead::copy(StringHead* other)
{
  cell = other.cell;
  nChar = other.nChar;
  memcpy(body(), other.body(), nChar);
}

StringHead* StringHead::next()
{
  (StringHead*) ((char* this) + nAlloc());
}

// Total number of bytes allocated to this string
int StringHead::nAlloc()
{
  return sizeof(StringHead) + 4*(nChar / 4);
}

// ----------------------------------------------------------

//
// StringSpace object class
//

StringSpace::StringSpace(int nBytes)
{
  start = (StringHead*) malloc(nBytes);
  frontier = start;
  end = (void*) start + nBytes;
}

StringHead* StringSpace::alloc(char* s, Cell* c)
{
  // TODO: Check for space exceeded
  
  StringHead* p = frontier;
  p->cell = c;
  p->nChar = strlen(s);
  memcpy(p->body(), s, p->nChar);

  ++nStrings;
  frontier = p->next();
  return p;
}

void StringSpace::compactify()
{
  StringHead* p = start;
  StringHead* front = start;   // Temporary frontier
  int nDeleted = 0;

  for (int i = 0; i < nStrings; ++i)
    {
      // Save pointer to next, in case *p contents overwritten
      // by copy operation
      q = p.next();

      if (p.isMarked())
	{
	  front.copy(p);
	  front.unmark()
	  front = front.next();
	}
      else
	{
	  ++nDeleted;
	}
      p = q;
    }

  nStrings = nStrings - nDeleted;
  frontier = front;
}


