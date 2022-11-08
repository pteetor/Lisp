//
// Dict object class implementation
//
// This particular implementation of the API
// is a simple-minded linear list.
//
#include "globals.h"
#include "Object.h"
#include "ObjPool.h"
#include "StringFinder.h"
#include "Dict.h"
#include "Heap.h"

Dict::Dict(ObjPool* op) : obj(op)
{
  root = obj->nil();
}

Object* Dict::find(Object* k)
{
  Object* p = root;
  while (p->nonNull())
    {
      if (key(p)->eq(k))
	return p;
      p = next(p);
    }
  return obj->nil();
}

void Dict::insert(Object* k, Object* v)
{
  Object* node = obj->cons(k, v);
  root = obj->cons(node, root);
}

Object* Dict::key(Object* node)
{
  return node->car()->car();
}

Object* Dict::lookup(Object* k)
{
  Object* p = find(k);
  return (p->nonNull() ? value(p) : obj->nil());
}

Object* Dict::next(Object* node)
{
  return node->cdr();
}

void Dict::remove(Object* k)
{
  if (root->null())
    return;
  
  if (key(root)->eq(k)) {
    root = next(root);
  } else {
    Object* prev = root;
    Object* p = next(root);
    while (p->nonNull())
      {
	if (key(p)->eq(k)) {
	  prev->replacd(next(p));
	  return;
	}
	p = next(p);
      }
  }
}

Object* Dict::value(Object* node)
{
  return node->car()->cdr();
}
