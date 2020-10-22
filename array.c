/* Array Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "array.h"

/* Forward Declaration of private Functions */

static int set(struct _array *this, void *e, const size_t bytes, const int index);
static void delete(void **e, void (*ffp)(void *fp));
static inline void sync(struct _array *this);

/* --- Public --- */

struct _array* array_ctor(const int size, const void *ffp, const void *cfp) {

    struct _array *array;

    if((array = malloc(sizeof(struct _array))) == NULL)
        return NULL;

    array->block = malloc(size * sizeof(void*));

    for(int i = 0; i < size; i++)
        array->block[i] = NULL;

    array->ffp = ffp;
    array->cfp = cfp;

    array->size = size;
    array->psize = size;

    array->top = 0;

    return array;
}

struct _array* array_cpy(const struct _array *src) {

    return NULL;
}

void array_dtor(struct _array *this) {

    sync(this);

    for(int i = 0; i < this->size; i++)  {

        if(this->block[i] == NULL)
            continue;

        delete(&this->block[i], this->ffp);
    }

    free(this->block);
    free(this);
}

int array_push(struct _array *this, void *value, const size_t bytes) {

    sync(this);
    set(this, value, bytes, this->top);

    this->top += 1;
}

void* array_pop(struct _array *this) {

    sync(this);
    delete(&this->block[this->top - 1], this->ffp);

    this->top -= 1;
}

int array_set(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);
    set(this, value, bytes, index);

    return 0;
}

int array_delete(struct _array *this, const int index) {

    sync(this);
    delete(&this->block[index], this->ffp);

    return 0;
}

int array_insert(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);

    for(int i = this->size - 1; i > index;  i--) {

        delete(&this->block[i], this->ffp);
        set(this, this->block[i - 1], bytes, i);
    }

    delete(&this->block[index], this->ffp);
    set(this, value, bytes, index);

    return 0;
}

void* array_at(const struct _array *this, const int index) {

    return this->block[index];
}

/* --- Private --- */

static int set(struct _array *this, void *e, const size_t bytes, const int index) {

    if(this->cfp != NULL) {

        this->block[index] = (void*) (*this->cfp)(e);

    } else {

        this->block[index] = malloc(bytes);
        memcpy(this->block[index], e, bytes);
    }

    return 0;
}

static void delete(void **e, void (*ffp)(void *fp)) {

    if(ffp != NULL)
        (*ffp)(*e);
    else
        free(*e);

    *e = NULL;
}

static inline void sync(struct _array *this) {

    if(this->psize == this->size)
        return;

    const size_t bytes = this->size * sizeof(void*);

    this->block = realloc(this->block, bytes);
    this->psize = this->size;
}
