/*
 * Stack (LIFO) backed by a fixed size array.
 *
 * top is the index of the last pushed item, so -1 means "empty".
 * pop() and peek() report success through a return code and write the value
 * to an out parameter, so a legitimate -1 in the stack is not mistaken for an
 * error.
 */

#include <assert.h>
#include <stdio.h>

#define STACK_CAPACITY 100

struct Stack {
    int data[STACK_CAPACITY];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_is_empty(const struct Stack *s) {
    return s->top == -1;
}

int stack_is_full(const struct Stack *s) {
    return s->top == STACK_CAPACITY - 1;
}

int stack_size(const struct Stack *s) {
    return s->top + 1;
}

/* Pushes a value. Returns 1 on success, 0 if the stack is full. */
int stack_push(struct Stack *s, int data) {
    if (stack_is_full(s)) {
        printf("stack_push: stack is full\n");
        return 0;
    }
    s->top++;
    s->data[s->top] = data;
    return 1;
}

/* Removes the top value into *out. Returns 1 on success, 0 if empty. */
int stack_pop(struct Stack *s, int *out) {
    if (stack_is_empty(s)) {
        printf("stack_pop: stack is empty\n");
        return 0;
    }
    *out = s->data[s->top];
    s->top--;
    return 1;
}

/* Reads the top value without removing it. Returns 1 on success, 0 if empty. */
int stack_peek(const struct Stack *s, int *out) {
    if (stack_is_empty(s)) {
        printf("stack_peek: stack is empty\n");
        return 0;
    }
    *out = s->data[s->top];
    return 1;
}

void stack_reset(struct Stack *s) {
    s->top = -1;
}

/* Prints the stack from top to bottom. */
void stack_print(const struct Stack *s) {
    if (stack_is_empty(s)) {
        printf("(empty)\n");
        return;
    }
    for (int i = s->top; i >= 0; i--) {
        printf("%d -> ", s->data[i]);
    }
    printf("(bottom)\n");
}

int main(void) {
    struct Stack s;
    int value = 0;

    printf("== Array stack ==\n");

    stack_init(&s);
    assert(stack_is_empty(&s));
    assert(stack_size(&s) == 0);
    stack_print(&s); /* must not crash while empty */

    assert(stack_pop(&s, &value) == 0);  /* pop on empty fails cleanly */
    assert(stack_peek(&s, &value) == 0); /* peek on empty fails cleanly */

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    printf("after push 10, 20, 30 : ");
    stack_print(&s);
    assert(stack_size(&s) == 3);
    assert(!stack_is_empty(&s));

    assert(stack_peek(&s, &value) == 1);
    assert(value == 30);
    assert(stack_size(&s) == 3); /* peek does not remove */

    assert(stack_pop(&s, &value) == 1);
    assert(value == 30); /* last in, first out */
    printf("after one pop         : ");
    stack_print(&s);
    assert(stack_size(&s) == 2);

    assert(stack_pop(&s, &value) == 1 && value == 20);
    assert(stack_pop(&s, &value) == 1 && value == 10);
    assert(stack_is_empty(&s));
    assert(stack_pop(&s, &value) == 0);

    /* Fill the stack to its capacity and check the overflow guard. */
    for (int i = 0; i < STACK_CAPACITY; i++) {
        assert(stack_push(&s, i) == 1);
    }
    assert(stack_is_full(&s));
    assert(stack_size(&s) == STACK_CAPACITY);
    assert(stack_push(&s, 999) == 0); /* one push too many */
    assert(stack_size(&s) == STACK_CAPACITY);

    stack_reset(&s);
    assert(stack_is_empty(&s));
    assert(stack_size(&s) == 0);
    printf("after reset           : ");
    stack_print(&s);

    printf("All array stack tests passed.\n");
    return 0;
}
