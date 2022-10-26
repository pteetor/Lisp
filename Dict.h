//
// Dict - dictionary object class
//
class Dict {
  Heap& heap;
  Object* root;

  static Object* key(Object* node);
  static Object* value(Object* node);
  static Object* next(Object* node);

  // Find a node within the dictionary
  Object* find(Object* k);
  
 public:
  Dict(Heap& h);

  Object* lookup(Object* k);
  void insert(Object* k, Object* v);
  void remove(Object* k);
};
