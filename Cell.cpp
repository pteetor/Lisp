#include "Heap.h"

Cell *makeSymbol(Cell *cp, std::string *s)
{
  cp->tag = SYMBOL_TAG;
  cp->string_p = s;
  return cp;
}

int length(Cell* list) {
  int len = 0;
  Cell *p = list;
  while (!p->null()) {
    ++len;
    p = p->cdr();
  }
  return len;
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
      os << *(ap->string_p);
      break;
    case SYMBOL_TAG:
      os << *(ap->string_p);
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
  } else if (p->isAtomic()) {
    printAtom(p, os);
  } else {
    os << "(";
    printSExpr(p->car(), os);
    
    p = p->cdr();
    while (p->isCons()) {
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
