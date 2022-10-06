//
// Simple unit tests for Cell object class
//

#include "Heap.h"

// ----------------------------------------------------------

int main() {
  Cell intCell(3);

  cout << endl;
  cout << "intCell.null() = " << intCell.null() << endl;
  cout << "atom(intCell) = " << intCell.atom() << endl;
  cout << "consp(intCell) = " << intCell.consp() << endl;
  cout << "integerp(intCell) = " << intCell.integerp() << endl;
  cout << "(int) intCell = " << (int) intCell << endl;
  cout << "(double) intCell = " << (double) intCell << endl;

  Cell dblCell(3.1415);

  cout << endl;
  cout << "(int) dblCell = " << (int) dblCell << endl;
  cout << "(double) dblCell = " << (double) dblCell << endl;

  Cell consCell(new Cell, new Cell);

  cout << endl;
  cout << "consCell.null() = " << consCell.null() << endl;
  cout << "consCell.atom() = " << consCell.atom() << endl;
  cout << "consCell.consp() = " << consCell.consp() << endl;

  try {
    cout << "(int) consCell = " << (int) consCell << endl;
  }
  catch (const std::bad_cast &e) {
    cout << endl << "Caught bad cast" << endl;
  }

  Cell markedCell;
  markedCell.mark();

  cout << endl;
  cout << "intCell.isMarked() = " << intCell.isMarked() << endl;
  cout << "markedCell.isMarked() = " << markedCell.isMarked() << endl;  
}
