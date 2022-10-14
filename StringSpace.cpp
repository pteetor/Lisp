//
//  Implementation of StringSpace
//

#include <cstring>
#include <iostream>

#include "globals.h"
#include "StringSpace.h"
#include "Heap.h"

//
// Global variable definitions
//
StringSpace theSpace(1000);

//
//  StringHead object class
//

void StringHead::init(Cell* c, const char* s)
{
  bMark = false;
  nChar = strlen(s);
  cell = c;
}

char* StringHead::body() const
{
  return (char*) this + sizeof(StringHead);
}

void StringHead::copy(StringHead* other)
{
  cell = other->cell;
  nChar = other->nChar;
  memcpy(body(), other->body(), nChar);
}

StringHead* StringHead::next() const
{
  return (StringHead*) (((char*) this) + nAlloc());
}

StringHead* StringHead::next(const char *s) const
{
  return (StringHead*) (((char*) this) + nRequired(s));
}

// Total number of bytes allocated to this string
int StringHead::nAlloc() const
{
  return sizeof(StringHead) + 4*((nChar + 3) / 4);
}

void StringHead::mark()
{
  bMark = true;
}

void StringHead::unmark()
{
  bMark = false;
}

bool StringHead::isMarked() const
{
  return (bool) bMark;
}

std::ostream& operator<<(std::ostream& os, const StringHead& h)
{
  os.write(h.body(), h.nChar);
  return os;
}

// ----------------------------------------------------------

//
// Static methods
//

// Total number of bytes needed to contain a string.
// We must account for
//   (1) The string header, and
//   (2) Round up to word boundary
int StringHead::nRequired(const char* s)
{
  return sizeof(StringHead) + 4*((strlen(s) + 3) / 4);
}

// ----------------------------------------------------------

//
// StringSpace object class
//

StringSpace::StringSpace(int nBytes)
{
  availBytes = nBytes;
  nStrings = 0;
  start = (StringHead*) new char[nBytes];
  frontier = start;
  end = (StringHead*) ((char*) start + nBytes);
}

StringSpace::~StringSpace()
{
  delete start;
}

StringHead* StringSpace::alloc(Cell* c, const char* s)
{
  StringHead* p = frontier;
    
  // Check for space exceeded
  if (p->next(s) > end)
    throw std::bad_alloc();

  p->init(c, s);
  memcpy(p->body(), s, p->nChar);

  availBytes = availBytes - p->nAlloc();
  ++nStrings;
  frontier = p->next();
  return p;
}

void StringSpace::compactify()
{
  StringHead* p = start;
  StringHead* q;
  StringHead* front = start;   // Temporary frontier
  int nDeleted = 0;

  for (int i = 0; i < nStrings; ++i)
    {
      // Save pointer to next, in case *p contents overwritten
      // by copy operation
      q = p->next();

      if (p->isMarked()) {
	front->copy(p);
	front->unmark();
	front->cell->set(front);
	front = front->next();
      }
      else {
	availBytes = availBytes + p->nAlloc();
	++nDeleted;
      }
      p = q;
    }

  nStrings = nStrings - nDeleted;
  frontier = front;
}
