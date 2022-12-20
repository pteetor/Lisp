//
// Lisp exception support
//

#include "globals.h"

LispEx::LispEx(ExceptionCode c) : code(c)
{
  // nop
}

const char* LispEx::what() const throw ()
{
  switch (code) {
  case A8:
    return "A8. Undefined variable";
  case A9:
    return "A9. Function object has no definition";
  case F2:
    return "F2. Argument list too long";
  case F3:
    return "F3. Argument list too short";
  case GC1:
    return "GC1. Fatal error in garbage collection";
  case I3:
    return "I3. Numeric conversion error";
  case X1:
    return "X1. car: Not a cons cell";
  case X2:
    return "X2: cdr: Not a cons cell";
  default:
    return "Holy cow! An exception!";
  }
}
