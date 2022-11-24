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

typedef Object* NativeFunction(Object* a, Heap& heap);
