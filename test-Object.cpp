//
// Simple unit tests for Object object class
//

#include "Object.h"
#include "Heap.h"

// ----------------------------------------------------------

int main() {
  Object intObject(3);

  cout << endl;
  cout << "intObject.null() = " << intObject.null() << endl;
  cout << "atom(intObject) = " << intObject.atom() << endl;
  cout << "consp(intObject) = " << intObject.consp() << endl;
  cout << "integerp(intObject) = " << intObject.integerp() << endl;
  cout << "(int) intObject = " << (int) intObject << endl;
  cout << "(double) intObject = " << (double) intObject << endl;

  Object dblObject(3.1415);

  cout << endl;
  cout << "(int) dblObject = " << (int) dblObject << endl;
  cout << "(double) dblObject = " << (double) dblObject << endl;

  Object consObject(new Object, new Object);

  cout << endl;
  cout << "consObject.null() = " << consObject.null() << endl;
  cout << "consObject.atom() = " << consObject.atom() << endl;
  cout << "consObject.consp() = " << consObject.consp() << endl;

  try {
    cout << "(int) consObject = " << (int) consObject << endl;
  }
  catch (const std::bad_cast &e) {
    cout << endl << "Caught bad cast" << endl;
  }

  Object markedObject;
  markedObject.mark();

  cout << endl;
  cout << "intObject.isMarked() = " << intObject.isMarked() << endl;
  cout << "markedObject.isMarked() = " << markedObject.isMarked() << endl;  
}
