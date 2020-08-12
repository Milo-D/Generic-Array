/* Array Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Project Headers
#include "array.h"

struct _private {

    int size;
    void **block;

    void (*ffp)(void *fp);
    void* (*cfp)(void *cp);
};

/* Forward Declaration of private Functions */

static int set(struct _array *this, void *e, const size_t bytes, const int index);
static void delete(void **e, void (*ffp)(void *fp));
static inline void sync(struct _array *this);

/* --- Public --- */

struct _array* array_ctor(const int size, const void *ffp, const void *cfp) {

    struct _array *array;

    if((array = malloc(sizeof(struct _array))) == NULL)
        return NULL;

    if((array->p = malloc(sizeof(struct _private))) == NULL) {

        free(array);
        return NULL;
    }

    array->p->block = malloc(size * sizeof(void*));

    for(int i = 0; i < size; i++)
        array->p->block[i] = NULL;

    array->p->ffp = ffp;
    array->p->cfp = cfp;

    array->p->size = size;
    array->size = size;

    array->top = 0;

    return array;
}

struct _array* array_cpy(const struct _array *src) {

    return NULL;
}

void array_dtor(struct _array *this) {

    sync(this);

    for(int i = 0; i < this->size; i++)  {

        if(this->p->block[i] == NULL)
            continue;

        delete(&this->p->block[i], this->p->ffp);
    }

    free(this->p->block);
    free(this->p);
    free(this);
}

int array_push(struct _array *this, void *value, const size_t bytes) {

    sync(this);
    set(this, value, bytes, this->top);

    this->top += 1;
}

void* array_pop(struct _array *this) {

    sync(this);
    delete(&this->p->block[this->top - 1], this->p->ffp);

    this->top -= 1;
}

int array_set(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);
    set(this, value, bytes, index);

    return 0;
}

int array_delete(struct _array *this, const int index) {

    sync(this);
    delete(&this->p->block[index], this->p->ffp);

    return 0;
}

int array_insert(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);

    for(int i = this->size - 1; i > index;  i--) {

        delete(&this->p->block[i], this->p->ffp);
        set(this, this->p->block[i - 1], bytes, i);
    }

    delete(&this->p->block[index], this->p->ffp);
    set(this, value, bytes, index);

    return 0;
}

void* array_at(const struct _array *this, const int index) {

    return this->p->block[index];
}

/* --- Private --- */

static int set(struct _array *this, void *e, const size_t bytes, const int index) {

    if(this->p->cfp != NULL) {

        this->p->block[index] = (void*) (*this->p->cfp)(e);

    } else {

        this->p->block[index] = malloc(bytes);
        memcpy(this->p->block[index], e, bytes);
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

    if(this->size == this->p->size)
        return;

    const size_t bytes = this->size * sizeof(void*);

    this->p->block = realloc(this->p->block, bytes);
    this->p->size = this->size;
}
