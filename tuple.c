/* Tuple Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers
#include "tuple.h"

struct _private {

    void **value;
    size_t *bytes;

    DTYPE *type;
};

extern struct _tuple* tuple_ctor(const int n, ...) {

    struct _tuple *tuple;

    if(n < 1) 
        return NULL;

    if((tuple = malloc(sizeof(struct _tuple))) == NULL)
        return NULL;
    
    printf("malloc (tuple) at tuple_ctor -> %p\n", tuple);

    if((tuple->p = malloc(sizeof(struct _private))) == NULL) {

        free(tuple);
        return NULL;
    }

    printf("malloc (tuple->p) at tuple_ctor -> %p\n", tuple->p);
    
    va_list var;
    va_start(var, n);
    
    tuple->p->type = malloc(n * sizeof(DTYPE));
    printf("malloc (tuple->p->type) at tuple_ctor -> %p\n", tuple->p->type);
    
    tuple->p->value = malloc(n * sizeof(void*));
    printf("malloc (tuple->p->value) at tuple_ctor -> %p\n", tuple->p->value);
    
    tuple->p->bytes = malloc(n * sizeof(size_t));
    printf("malloc (tuple->p->bytes) at tuple_ctor -> %p\n", tuple->p->bytes);

    for(int i = 0; i < n; i++)
        tuple->p->type[i] = va_arg(var, DTYPE);

    for(int i = 0; i < n; i++) {

        tuple->p->value[i] = NULL;
        tuple->p->bytes[i] = 0;
    }

    tuple->size = n;
    va_end(var);
    
    return tuple;
}

extern struct _tuple* tuple_cpy(const struct _tuple *src) {

    struct _tuple *dest;

    if(src == NULL)
        return NULL;

    if((dest = malloc(sizeof(struct _tuple))) == NULL)
        return NULL;
    
    printf("malloc (dest) at tuple_cpy -> %p\n", dest);

    if((dest->p = malloc(sizeof(struct _private))) == NULL) {

        free(dest);
        return NULL;
    }
    
    printf("malloc (dest->p) at tuple_cpy -> %p\n", dest->p);

    dest->p->type = malloc(src->size * sizeof(DTYPE));
    printf("malloc (dest->p->type) at tuple_cpy -> %p\n", dest->p->type);
    
    dest->p->value = malloc(src->size * sizeof(void*));
    printf("malloc (dest->p->value) at tuple_cpy -> %p\n", dest->p->value); 
    
    dest->p->bytes = malloc(src->size * sizeof(size_t));
    printf("malloc (dest->p->bytes) at tuple_cpy -> %p\n", dest->p->bytes);

    dest->size = src->size;

    for(int i = 0; i < src->size; i++) {
        
        dest->p->type[i] = src->p->type[i];
        dest->p->value[i] = NULL;

        tuple_set(dest, src->p->value[i], src->p->bytes[i], i);
    }

    return dest;
}

extern void tuple_dtor(struct _tuple *this) {
    
    for(int i = 0; i < this->size; i++) {

        printf("free (this->p->value[i]) at tuple_dtor -> %p\n", this->p->value[i]);
        
        if(this->p->value[i] != NULL)
            free(this->p->value[i]);
    }
    
    printf("free (this->p->value) at tuple_dtor -> %p\n", this->p->value);
    free(this->p->value);

    printf("free (this->p->bytes) at tuple_dtor -> %p\n", this->p->bytes);
    free(this->p->bytes);
    
    printf("free (this->p->type) at tuple_dtor -> %p\n", this->p->type);
    free(this->p->type);
    
    printf("free (this->p) at tuple_dtor -> %p\n", this->p);
    free(this->p);
    
    printf("free (this) at tuple_dtor -> %p\n", this);
    free(this);
}

extern int tuple_set(struct _tuple *this, const void *value, const size_t size, const int index) {

    if(this == NULL)
        return -1;

    if(index < 0 || index >= this->size)
        return -1;

    if(this->p->value[index] != NULL) {
        
        printf("free (this->p->value[index]) at tuple_set -> %p\n", this->p->value[index]);
        free(this->p->value[index]);
    }

    this->p->value[index] = malloc(size);
    printf("malloc (this->p->value[index]) at tuple_ctor -> %p\n", this->p->value[index]);
    
    memcpy(this->p->value[index], value, size);
    this->p->bytes[index] = size;

    return 0;
}

extern void* tuple_get(const struct _tuple *this, const int index) {

    if(index < 0 || index >= this->size)
        return NULL;

    return this->p->value[index];
}

extern DTYPE tuple_type(const struct _tuple *this, const int index) {

    if(index < 0 || index >= this->size)
        return VOID;

    return this->p->type[index];    
}




