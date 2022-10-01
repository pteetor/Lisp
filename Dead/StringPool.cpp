//
// StringPool implementation
//
#include "Heap.h"

StringPool stringPool;

const string* uniqueStr(const string& s)
{
  stringPool.insert(s);
  StringPool::iterator it = stringPool.find(s);
  const string& uniq = *it;
  return &uniq;
}

const string* uniqueStr(const char *s) {
  return uniqueStr(string(s));
}
