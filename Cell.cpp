//
// Implementation of Cell object class
//
#include <ostream>

#include "StringSpace.h"
#include "Heap.h"

//
// Cell - constructors
//

Cell::Cell(const char *s) {
  tag = STRING_TAG;
  strhead = theSpace.alloc(this, s);
}

//
// Cell - private methods
//

void Cell::checkTag(Tag t) {
  if (this->tag != t) {
    throw std::bad_cast();
  };
}

//
// Cell - public methods
//

Cell* Cell::set(const char *s) {
  tag = STRING_TAG;
  strhead = theSpace.alloc(this,s);
  return this;
}

Cell* Cell::setSymbol(const char *s) {
  tag = SYMBOL_TAG;
  strhead = theSpace.alloc(this,s);
  return this;
}

//
// Cell - non-member functions
//

Cell *makeSymbol(Cell *cp, const char* s)
{
  // TODO: Look for duplicate symbol or duplicate string
  
  cp->tag = SYMBOL_TAG;
  cp->strhead = theSpace.alloc(cp, s);
  return cp;
}

void printAtom(const Cell *ap, ostream& os) {  
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

void printSExpr(const Cell* p, ostream& os)
{
  if (p->null()) {
    os << "()";
  } else if (p->atom()) {
    printAtom(p, os);
  } else {
    os << "(";
    printSExpr(p->car(), os);
    
    p = p->cdr();
    while (p->consp()) {
      os << " ";
      printSExpr(p->car(), os);
      p = p->cdr();
    }
    if (p->null()) {
      os << ")";
    } else {
      os << " . ";
      printSExpr(p, os);
      os << ")";
    }
  }
}

ostream& operator<<(ostream& os, const Cell& c) {
  printSExpr(&c, os);
  return os;
}

void Cell::dump()
{
  cout << "[ " << std::hex << this << " ]"
       << "  mark bit: " << markBit() << endl << std::dec;
  if (atom()) {
    cout << "atom: " << tagName(pureTag()) << " (" << pureTag() << ")" << endl;
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
