//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include <sstream>
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Heap.h"
#include "Tokenizer.h"
#include "Reader.h"

using namespace std;

int main() {
  std::string theInput = "( (lambda (x pwr) (+ x (** 2 pwr))) 10 3.14)";

  std::cout << "Input is: " << theInput << std::endl;

  ObjPool pool(1000);
  StringSpace space(1000);
  StringFinder finder(&pool, &space);
  Heap heap(&pool, &finder);
  
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);
  Reader rdr(tkz, heap);

  tkz.traceOn();
  bool ok = rdr.read();

  if (ok) {
    cout << endl;
    // heap.dump();
  
    print(heap.top());
    cout << endl;
  }
  
  return(0);
}
