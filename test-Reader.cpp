//
// Simple unit tests of Tokenizer classes
//

#include <iostream>
#include <sstream>
#include "StringSpace.h"
#include "Heap.h"
#include "tokenizer.h"
#include "Reader.h"

using namespace std;

int main() {
  std::string theInput = "( (lambda (x pwr) (+ x (** 2 pwr))) 10 3.14)";

  std::cout << "Input is: " << theInput << std::endl;

  StringSpace space(1000);
  Heap heap(1000, &space);
  
  std::stringstream theStream(theInput);
  Tokenizer tkz(theStream);
  Reader rdr(tkz, heap);

  tkz.traceOn();
  auto se = rdr.read();

  cout << endl;
  heap.dump();
  
  print(se);
  cout << endl;
  
  return(0);
}
