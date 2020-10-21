/* Array Header */

#ifndef ARRAY_H
#define ARRAY_H

// C Headers
#include <inttypes.h>

typedef struct _array {

    // public Members
    int top;
    int size;

    // private Members
    int psize;
    void **block;

    void (*ffp)(void *fp);
    void* (*cfp)(void *cp);

} array_t;

extern struct _array* array_ctor();
extern struct _array* array_cpy(const struct _array *src);
extern void array_dtor(struct _array *this);

extern int array_push(struct _array *this, void *value, const size_t bytes);
extern void* array_pop(struct _array *this);

extern int array_set(struct _array *this, void *value, const size_t bytes, const int index);
extern int array_delete(struct _array *this, const int index);
extern int array_insert(struct _array *this, void *value, const size_t bytes, const int index);

extern void* array_at(const struct _array *this, const int index);

#endif
