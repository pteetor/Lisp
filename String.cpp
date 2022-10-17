//
//  Implementation of StringSpace
//

#include <cstring>
#include <iostream>

#include "globals.h"
#include "Object.h"
// No?: #include "Heap.h"

//
//  String object class
//

int String::init(const char* s, Object* c)
{
  bMark = false;
  nChar = strlen(s);
  cell = c;
  memcpy(body(), s, nChar);

  return nAlloc();
}

String* String::set(Object* c)
{
  cell = c;
  return this;
}

char* String::body() const
{
  return (char*) this + sizeof(String);
}

void String::copy(String* other)
{
  cell = other->cell;
  nChar = other->nChar;
  memcpy(body(), other->body(), nChar);
}

String* String::next() const
{
  return (String*) (((char*) this) + nAlloc());
}

String* String::next(const char *s) const
{
  return (String*) (((char*) this) + nRequired(s));
}

// Total number of bytes allocated to this string
int String::nAlloc() const
{
  return sizeof(String) + 4*((nChar + 3) / 4);
}

void String::mark()
{
  bMark = true;
}

void String::unmark()
{
  bMark = false;
}

bool String::isMarked() const
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
int String::nRequired(const char* s)
{
  return sizeof(String) + 4*((strlen(s) + 3) / 4);
}

std::ostream& operator<<(std::ostream& os, const String& h)
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
  start = (String*) new char[nBytes];
  frontier = start;
  end = (String*) ((char*) start + nBytes);
}

StringSpace::~StringSpace()
{
  delete start;
}

String* StringSpace::alloc(const char* s, Object* c)
{
  // TODO: Look for duplicate string

  String* p = frontier;
  String* q = p->next(s);
    
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
  String* p = start;
  String* q;
  String* front = start;   // Temporary frontier
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
