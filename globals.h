//
// Forward declarations and global declarations
//

// Standard lib stuff used commonly
#include <cassert>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <ostream>

class Object;
class ObjPool;
class String;
class StringSpace;
class Dict;
class Heap;

typedef void NativeFunction(int nArgs, Object** args, Heap& heap);

typedef enum ExceptionCodeEnum {
  // Interpreter errors
  A1,
  A2,
  A3,
  A4,
  A5,
  A6,
  A7,
  A8,
  A9,

  // Misc. errors
  F1,
  F2,
  F3,
  F4,
  F5,
  G1,
  G2,

  // Garbage collector errors
  GC1,
  GC2,

  // Arithmetic errors
  I1,
  I2,
  I3,
  I4,

  // I/O errors
  P1,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,

  // Local errors
  X1,     // car of atom
  X2      // cdr of atom
} ExceptionCode;

struct LispEx : public std::exception {
  LispEx(ExceptionCode);

  ExceptionCode code;
  
  const char * what () const throw ();
};
