#include "wfclib_stack.h"
#include <stdlib.h>
#include <string.h>

stack_t *wfclib_stack_init(unsigned int max_size) {
    stack_t *new_stack;
    new_stack = calloc(1, sizeof(stack_t));

    if (new_stack) {
        new_stack->element = calloc(max_size, sizeof(void *));

        if (new_stack->element) {
            new_stack->size = 0;
            memcpy((unsigned int *)&new_stack->capacity, &max_size, sizeof(unsigned int));
        } else {
            free(new_stack);
            new_stack = NULL;
        }
    }

    return new_stack;
}

stack_err_t wfclib_stack_push(stack_t *stack, void *element) {
    
    stack_err_t rc = STACK_FULL;

    if (stack->size < stack->capacity) {
        stack->element[stack->size] = element;
        stack->size++;
        rc = STACK_OK;
    }

    return rc;
}

stack_err_t wfclib_stack_pop(stack_t *stack, void *element) {
    
    stack_err_t rc = STACK_EMPTY;

    if (stack->size > 0) {
        stack->size--;
        element = stack->element[stack->size];
        stack->element[stack->size] = NULL;
        rc = STACK_OK;
    }

    return rc;
}

unsigned int wfclib_stack_size(stack_t *stack) {

    return stack->size;
}

void *wfclib_stack_element(stack_t *stack, unsigned int index) {
     
     void *element = NULL;

    if (index < stack->size) {
        element = stack->element[index];
    }

     return element;
}

void wfclib_stack_free(stack_t *stack) {

    free(stack->element);
    free(stack);
}