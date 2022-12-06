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
  case F2:
    return "F2. Argument list too long";
  case F3:
    return "F3. Argument list too short";
  case GC1:
    return "GC1. Fatal error in garbage collection";
  case I3:
    return "I3. Numeric conversion error";
  default:
    return "Holy cow! An exception!";
  }
}
