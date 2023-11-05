#ifndef _WFCLIB_STACK_H_
#define _WFCLIB_STACK_H_

typedef enum {
    STACK_OK = 0,
    STACK_EMPTY,
    STACK_FULL
} stack_err_t;

typedef struct {
    void **element; /* Array of pointers */
    unsigned int size;
    const unsigned int capacity;
} stack_t;

stack_t *wfclib_stack_init(unsigned int max_size);
stack_err_t wfclib_stack_push(stack_t *stack, void *element);
stack_err_t wfclib_stack_pop(stack_t *stack, void *element);
unsigned int wfclib_stack_size(stack_t *stack);
void *wfclib_stack_element(stack_t *stack, unsigned int index);
void wfclib_stack_free(stack_t *stack);

#endif /* _WFCLIB_STACK_H_ */