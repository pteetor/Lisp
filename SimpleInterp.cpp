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

Object* SimpleInterp::eval(Object* e, Object* a)
{
  if (e->atom()) {
    if (e->symbolp()) {
      // TODO: Throw undefined-variable error
      return heap.nil();
    } else {
      return e;
    }
  } else {
    // cons case
    // TODO: Interpret function call
    return heap.nil();
  }

  // TODO: Throw internal error?
  heap.nil();
}
