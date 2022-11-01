//
// Implementation of Object object class
//
#include <cassert>
#include <cstring>
#include <ostream>

#include "Object.h"

//
// Object - constructors
//

// None

//
// Object - private methods
//

void Object::checkTag(Tag t) {
  if (this->tag != t) {
    throw std::bad_cast();
  };
}

//
// Object - public methods
//

bool Object::equal(const char* s) const
{
  switch (tag) {
  case STRING_TAG:
    return pstring->equal(s);
  case SYMBOL_TAG:
    // TODO: Find print-name; compare to s
    return false;
  default:
    return false;
  }
}

Object* Object::set(String* p)
{
  tag = STRING_TAG;
  pstring = p;
  return this;
}

Object* Object::set(Object* p)
{
  tag = SYMBOL_TAG;
  plist = p;
  return this;
}

Object* Object::set(Object *a, Object *d)
{
  car_p = a;
  cdr_p = d;
  return this;
}

void Object::mark() {
  Tag t = tag;
  tag = tag | MARK_BIT;
  if (t == STRING_TAG)
    pstring->mark();
  else if (t == SYMBOL_TAG) {
    plist->mark();
  }
}

void Object::unmark() {
  tag = tag & ~MARK_BIT;
}

//
// Object - non-member functions
//

void printAtom(const Object *ap, ostream& os) {  
  switch (ap->tag) {
  case NIL_TAG:
    os << "()";
    break;
  case BOOL_TAG:
    os << (ap->bool_v ? "*T*" : "*F*");
    break;
  case CHAR_TAG:
    os << ap->char_v;
    break;
  case INT_TAG:
    os << ap->int_v;
    break;
  case DOUBLE_TAG:
    os << ap->double_v;
    break;
  case STRING_TAG:
    os << *(ap->pstring);
    break;
  case SYMBOL_TAG:
    os << "[symbol]";   // TODO - find and print print-name property
    break;
  default:
    os << "???";
    break;
  }
}

void print(const Object* p, ostream& os)
{
  if (p->null()) {
    os << "()";
  } else if (p->atom()) {
    printAtom(p, os);
  } else {
    os << "(";
    print(p->car(), os);
    
    p = p->cdr();
    while (p->consp()) {
      os << " ";
      print(p->car(), os);
      p = p->cdr();
    }
    if (p->null()) {
      os << ")";
    } else {
      os << " . ";
      print(p, os);
      os << ")";
    }
  }
}

ostream& operator<<(ostream& os, const Object& c) {
  print(&c, os);
  return os;
}

void Object::dump()
{
  int nUse;
  
  cout << "[ " << std::hex << this << " ]"
       << "  mark bit: " << markBit() << endl << std::dec;
  if (atom()) {
    cout << "atom: " << tagName(pureTag()) << " (" << pureTag() << ") ";
    switch (pureTag()) {
    case STRING_TAG:
      cout << "len " << pstring->nChar << " ";;
      cout << "<";
      pstring->write(cout, 13);
      cout << ">";
      break;
    case SYMBOL_TAG:
      // TODO: Print print-name property (or "[unnamed]" if none)
      break;
    }
  } else {
    cout << std::hex << "cons: [ " << car()
	 << ", " << cdr() << " ]" << std::dec;
  }
  cout << endl;
}

// ----------------------------------------------------------

const char* tagName(const Tag t)
{
  switch (t) {
  case NIL_TAG: return "nil";
  case FREE_TAG: return "free";
  case BOOL_TAG: return "bool";
  case CHAR_TAG: return "char";
  case INT_TAG: return "int";
  case DOUBLE_TAG: return "dbl";
  case STRING_TAG: return "str";
  case SYMBOL_TAG: return "sym";
  default: return "???";
  }
}
