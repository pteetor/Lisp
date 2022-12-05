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
    return "Undefined variable";
  case I3:
    return "Numeric conversion error";
  default:
    return "Holy cow! An exception!";
  }
}
