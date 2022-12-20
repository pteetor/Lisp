//
// Frame of stack-based function arguments
//
// This is a simple object for managing function
// arguments when they are passed via the stack;
// and managing the return value (assumed to be
// a single object).
//
// The arguments are in a vector of consecutive
// stack locations. The return value replaces
// those arguments on the stack.
//

class Frame {
  Object** argvec;
  int n;

 public:
  Frame(int n, Object** vec);

  int nArgs() const;
  Object* arg(int i);

  void checkNArgs(int nParams);

  friend class Heap;
};

//
// Design idea:
//
// A UserFrame object would manage stack-based arguments
// and the return value for a user-defined function.
// The eval() function would build the UserFrame prior
// to invoking the user function.
//
// This would be especially useful for producing
// trace-back information.
//
class UserFrame {
  // - Pointer to vector of args
  // - nArgs
  // - Function object
  // - Pointer to previous UserFrame
};
