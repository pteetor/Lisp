//
// Implementation of Frame class
//

#include "globals.h"
#include "Frame.h"
#include "functions.h"

Frame::Frame(int nArgs, Object** vec)
{
  argvec = vec;
  n = nArgs;
}

void Frame::checkMacroArgs(int nParams)
{
  int nActual = length(arg(0));
  if (nActual > nParams)
    throw LispEx(F2);
  if (nActual < nParams)
    throw LispEx(F3);
}

void Frame::checkNArgs(int nParams)
{
  if (n > nParams)
    throw LispEx(F2);
  if (n < nParams)
    throw LispEx(F3);
}

int Frame::nArgs() const {
  return n;
}

Object* Frame::arg(int i) {
  return argvec[i];
}
