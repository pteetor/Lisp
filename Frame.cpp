//
// Implementation of Frame class
//

#include "globals.h"
#include "Frame.h"

Frame::Frame(int nArgs, Object** vec)
{
  argvec = vec;
  n = nArgs;
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
