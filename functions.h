//
// Global functions
//

extern int length(Object* list);
extern Object* member(Object* list, Object* elem);
extern Object* elide(Object* list, Object* elem);

extern void defineRunTimeFunctions(Heap& heap);

//
// Lisp run-time functions
//
// extern Function sum_f;
