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

    // printf("malloc (array) at array_ctor -> %p\n", array);

    if((array->p = malloc(sizeof(struct _private))) == NULL) {

        free(array);
        return NULL;
    }

    // printf("malloc (array->p) at array_ctor -> %p\n", array->p);

    array->p->block = malloc(size * sizeof(void*));

    // printf("malloc (array->p->block) at array_ctor -> %p\n", array->p->block);

    for(int i = 0; i < size; i++)
        array->p->block[i] = NULL;

    array->p->ffp = ffp;
    array->p->cfp = cfp;

    array->p->size = size;
    array->size = size;

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

        (*this->p->ffp)(this->p->block[i]);

       // delete(&this->p->block[i], this->p->ffp);
    }

    // printf("free (this->p->block) at array_dtor -> %p\n", this->p->block);
    free(this->p->block);

    // printf("free (this->p) at array_dtor -> %p\n", this->p);
    free(this->p);

    // printf("free (this) at array_dtor -> %p\n", this);
    free(this);
}

int array_set(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);
    // set(this, value, bytes, index);

    if((this->p->block[index] = malloc(bytes)) == NULL)
        return -1;

    // printf("malloc (this->p->block[%d]) at set -> %p\n", index, this->p->block[index]);

    if(this->p->cfp != NULL)
        this->p->block[index] = (void*) (*this->p->cfp)(value);
    else
        memcpy(this->p->block[index], value, bytes);

    return 0;
}

extern int array_delete(struct _array *this, const int index) {

    sync(this);
    delete(&this->p->block[index], this->p->ffp);

    return 0;
}

int array_insert(struct _array *this, void *value, const size_t bytes, const int index) {

    sync(this);
    return 0;
}

void* array_at(const struct _array *this, const int index) {

    return this->p->block[index];
}

/* --- Private --- */

static int set(struct _array *this, void *e, const size_t bytes, const int index) {

    if((this->p->block[index] = malloc(bytes)) == NULL)
        return -1;

    // printf("malloc (this->p->block[%d]) at set -> %p\n", index, this->p->block[index]);

    if(this->p->cfp != NULL)
        this->p->block[index] = (void*) (*this->p->cfp)(e);
    else
        memcpy(this->p->block[index], e, bytes);

    return 0;
}

static void delete(void **e, void (*ffp)(void *fp)) {

    if(ffp != NULL) {
        (*ffp)(*e);
    }
    else {

        // printf("free (e) at delete -> %p\n", *e);
        free(*e);
    }

    *e = NULL;
}

static inline void sync(struct _array *this) {

    if(this->size == this->p->size)
        return;

    const size_t bytes = this->size * sizeof(void*);

    this->p->block = realloc(this->p->block, bytes);
    this->p->size = this->size;
}
