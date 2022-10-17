//
// Implementation of Object object class
//
#include <ostream>

#include "Object.h"
// No?: #include "Heap.h"

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

Object* Object::set(String* p, Tag t)
{
  tag = t;
  strhead = p;
  return this;
}

Object* Object::set(Object *a, Object *d)
{
  car_p = a; cdr_p = d;
  return this;
}

void Object::mark() {
  if (tag == STRING_TAG || tag == SYMBOL_TAG)
    strhead->mark();
  tag = tag | MARK_BIT;
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
      os << *(ap->strhead);
      break;
    case SYMBOL_TAG:
      os << *(ap->strhead);
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
    case SYMBOL_TAG:
      nUse = (strhead->nChar <= 10 ? strhead->nChar : 10);
      cout << "len " << strhead->nChar << " " << '"';
      cout.write(strhead->body(), nUse);
      if (nUse < strhead->nChar)
	cout << "...";
      cout << '"' << endl;
      break;
    default:
      cout << endl;
      break;
    }
  } else {
    cout << std::hex << "car: " << car()
	 << "  cdr: " << cdr() << endl << std::dec;
  }
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
