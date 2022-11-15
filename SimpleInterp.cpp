//
// Simple Interp class
//

#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"

#include "Interp.h"

SimpleInterp::SimpleInterp(Heap& h) : heap(h)
{
  // nop
}

Object* SimpleInterp::eval(Object* e)
{
  // TBD
  return heap.nil();
}
