#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tuple.h"
#include "array.h"

void print(array_t *a) {

    for(int i = 0; i < a->size; i++) {

        const int value = *((int*) array_at(a, i));
        printf("%d ", value);
    }

    printf("\n");
}

void print_tuple(tuple_t *t) {

    const int n = *((int*) tuple_get(t, 0));
    const int m = *((int*) tuple_get(t, 1));

    printf("(%d, %d)\n", n, m);
}

void test_default(void) {

    array_t *a = array_ctor(10, NULL, NULL);

    for(int i = 0; i < 10; i++)
        array_set(a, &i, sizeof(int), i);

    print(a);
    array_dtor(a);

    exit(0);
}

void test_inc(void) {

    array_t *a = array_ctor(10, NULL, NULL);

    for(int i = 0; i < 10; i++)
        array_set(a, &i, sizeof(int), i);

    a->size += 10;

    for(int i = 10; i < 20; i++)
        array_set(a, &i, sizeof(int), i);

    print(a);
    array_dtor(a);
}

void test_string(void) {

    array_t *a = array_ctor(10, NULL, NULL);

    for(int i = 0; i < 10; i++) {

        char str[32] = "test";
        const size_t bytes = 5 * sizeof(char);

        array_set(a, str, bytes, i);
    }

    for(int i = 0; i < a->size; i++) {

        char *str = array_at(a, i);
        printf("%s\n", str);
    }

    array_dtor(a);
    exit(0);
}

void test_struct(void) {

    array_t *a = array_ctor(1, tuple_dtor, tuple_cpy);

    for(int i = 0; i < 1; i++) {

        tuple_t *t = tuple_ctor(2, INT, INT);

        tuple_set(t, &i, sizeof(int), 0);
        tuple_set(t, &i, sizeof(int), 1);

        array_set(a, t, sizeof(tuple_t), i);
        tuple_dtor(t);
    }

    for(int i = 0; i < a->size; i++) {

        tuple_t *t = array_at(a, i);
        print_tuple(t);
    }

    array_dtor(a);

    exit(0);
}

int main(void) {

    test_struct();

    return EXIT_SUCCESS;
}
