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

int StringHead::init(const char* s, Cell* c)
{
  bMark = false;
  nChar = strlen(s);
  cell = c;
  memcpy(body(), s, nChar);

  return nAlloc();
}

StringHead* StringHead::set(Cell* c)
{
  cell = c;
  return this;
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

std::ostream& operator<<(std::ostream& os, const StringHead& h)
{
  os.write(h.body(), h.nChar);
  return os;
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

StringHead* StringSpace::alloc(const char* s, Cell* c)
{
  // TODO: Look for duplicate string

  StringHead* p = frontier;
  StringHead* q = p->next(s);
    
  // Check for space exceeded
  if (q > end)
    throw std::bad_alloc();

  p->init(s, c);

  availBytes = availBytes - p->nAlloc();
  ++nStrings;
  frontier = q;
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

void StringSpace::dump()
{
  cout << "----- begin StringSpace -----" << endl;
 
  cout << "availBytes = " << availBytes << endl;
  cout << "nStrings = " << nStrings << endl;

  cout << "-----  end  StringSpace -----" << endl;
}
