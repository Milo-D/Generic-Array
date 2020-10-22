#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define N 1000000

typedef struct _test {

    int *mem;

} test_t;

test_t* test_ctor(const int begin) {

    test_t *test = malloc(sizeof(test_t));
    test->mem = malloc(sizeof(int) * 10);

    for(int i = begin; i < begin + 10; i++)
        test->mem[i] = i;

    return test;
}

test_t* test_cpy(const test_t *src) {

    test_t *dest = malloc(sizeof(test_t));
    dest->mem = malloc(sizeof(int) * 10);

    for(int i = 0; i < 10; i++)
        dest->mem[i] = src->mem[i];

    return dest;
}

void test_dtor(test_t *this) {

    free(this->mem);
    free(this);
}

void stress_test(const int iter) {

    array_t *a = array_ctor(iter, NULL, NULL);

    for(int i = 0; i < iter; i++)
        array_set(a, &i, sizeof(int), i);

    for(int i = 0; i < a->size; i++) {

        int val = *((int*) array_at(a, i));
    }

    array_dtor(a);
}

void test_struct(void) {

    array_t *a = array_ctor(10, test_dtor, test_cpy);

    for(int i = 0; i < 10; i++) {

        test_t *t = test_ctor(i * 10);

        array_set(a, t, sizeof(test_t), i);
        test_dtor(t);
    }

    array_dtor(a);
}

void test_insert(const int index) {

    array_t *a = array_ctor(10, NULL, NULL);

    for(int i = 0; i < 10; i++)
        array_set(a, &i, sizeof(int), i);

    int in = 21;
    array_insert(a, &in, sizeof(int), index);

    for(int i = 0; i < a->size; i++) {

        int *val = array_at(a, i);
        printf("[%d (pos: %d)] ", *val, i);
    }

    printf("\n");
    array_dtor(a);
}

int main(int argc, char **argv) {

    const int index = atoi(argv[1]);
    test_insert(index);

    return EXIT_SUCCESS;
}
