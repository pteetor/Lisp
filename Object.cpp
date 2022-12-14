//
// Implementation of Object object class
//
#include <cassert>
#include <cstring>
#include <ostream>

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"

//
// Object - constructors
//

// None

//
// Methods
//

bool Object::callablep() const
{
  return tag == FUNCTION_TAG || tag == MACRO_TAG;
}

void Object::callFunction(Frame& f, Heap& heap)
{
  assert(functionp());

  (function_p)(f, heap);
}

void Object::callMacro(Frame& f, Object* env, Heap& heap)
{
  assert(macrop());
  
  (macro_p)(f, env, heap);
}

Object* Object::car() const
{
  assert(consp() || isFree());

  return car_p;
}

Object* Object::cdr() const
{
  assert(consp() || isFree());

  return cdr_p;
}

void Object::checkTag(Tag t) {
  if (this->tag != t) {
    throw std::bad_cast();   // Generic error - should never happen
  };
}

void Object::dump()
{
  static const char* indent = "  ";
  
  cout << "[ " << std::hex << this << " ]"
       << "  mark bit: " << markBit() << endl << std::dec;
  cout << indent;
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
      cout << "<";
      print(pname_p);
      cout << ">";
      break;
    }
  } else {
    cout << std::hex << "cons: [ " << car()
	 << ", " << cdr() << " ]" << std::dec;
  }
  cout << endl;
}

bool Object::equal(const char* s) const
{
  switch (tag) {
  case STRING_TAG:
    return pstring->equal(s);
  case SYMBOL_TAG:
    return pname_p->pstring->equal(s);
  default:
    return false;
  }
}

Object* Object::pname() const
{
  assert(stringp());
  return pname_p;
}

Object* Object::set(String* p)
{
  tag = STRING_TAG;
  pstring = p;
  return this;
}

Object* Object::set(NativeFunction* p)
{
  tag = FUNCTION_TAG;
  function_p = p;
  return this;
}

Object* Object::set(NativeMacro* p)
{
  tag = MACRO_TAG;
  macro_p = p;
  return this;
}

Object* Object::set(Object* p, Tag t)
{
  tag = t;
  pname_p = p;
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
    pname_p->mark();
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
  case FREE_TAG:
    os << "[free]";
    break;
  case FUNCTION_TAG:
    os << "[function]";
    break;
  case MACRO_TAG:
    os << "[macro]";
    break;
  case CLOSURE_TAG:
    os << "[closure]";
    break;
  case SYMBOL_TAG:
    os << *(ap->pname_p->pstring);
    break;
  default:
    os << "[unknown-atom]";
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


// ----------------------------------------------------------

const char* tagName(const Tag t)
{
  switch (t) {
  case NIL_TAG: return "nil";
  case FREE_TAG: return "fre";
  case BOOL_TAG: return "bol";
  case CHAR_TAG: return "chr";
  case INT_TAG: return "int";
  case DOUBLE_TAG: return "dbl";
  case STRING_TAG: return "str";
  case SYMBOL_TAG: return "sym";
  default: return "???";
  }
}
