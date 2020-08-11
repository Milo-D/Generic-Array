/* Array Header */

#ifndef ARRAY_H
#define ARRAY_H

// C Headers
#include <inttypes.h>

struct _private;

typedef struct _array {

    int size;
    struct _private *p;

} array_t;

extern struct _array* array_ctor();
extern struct _array* array_cpy(const struct _array *src);
extern void array_dtor(struct _array *this);

extern int array_set(struct _array *this, void *value, const size_t bytes, const int index);
extern int array_delete(struct _array *this, const int index);
extern int array_insert(struct _array *this, void *value, const size_t bytes, const int index);

extern void* array_at(const struct _array *this, const int index);

#endif