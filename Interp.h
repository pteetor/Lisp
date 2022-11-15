//
// Semantic interpreter for Lisp
//

class AbstInterp {
public:
  virtual Object* eval(Object* e);
};

class EchoInterp {
 public:
  EchoInterp() { }

  Object* eval(Object* e) { return e; }
};

class SimpleInterp {
  Heap& heap;

public:
  SimpleInterp(Heap& h);
  Object* eval(Object* e);
};
