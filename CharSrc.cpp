//
// CharSrc implementation
//
#include "CharSrc.h"

bool CharSrc::first(char& ch) { return false; }

bool CharSrc::next(char& ch) { return false; }

bool CharSrc::now(char& ch) { return false; }

//
// StringSrc implementation
//
StringSrc::StringSrc(const char *s) : src(s)
{
  p = src;
}

bool StringSrc::first(char& ch)
{
  if (*p == 0) return false;
  ch = *p;
  return true;
}

bool StringSrc::next(char& ch)
{
  if (*p == 0) return false;
  ch = *(++p);
  return (ch != 0);
}

bool StringSrc::now(char& ch)
{
  if (*p == 0) return false;
  ch = *p;
  return true;
}
