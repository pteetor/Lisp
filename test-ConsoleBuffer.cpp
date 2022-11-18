//
// Unit test for ConsoleBuffer
//

#include <iostream>
#include "ConsoleBuffer.h"

using namespace std;

// TEST
#include <unistd.h>

int main()
{
  ConsoleBuffer cb;
  char ch;

  cout << "isatty(0) = " << isatty(0) << endl;
  cout << "isatty(1) = " << isatty(1) << endl;

  while (ch = cb.next())
    {
      cout << "'" << ch << "'" << endl;
    }
}
