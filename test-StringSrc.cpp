#include <iostream>
#include "CharSrc.h"

using namespace std;

int main()
{
  StringSrc cs("foo\nfum\n");
  char ch;

  if (cs.first(ch))
    {
      cout << "'" << ch << "'";
      while (cs.next(ch))
	{
	  cout << " '" << ch << "'";
	}
      cout << endl;
    }
}
