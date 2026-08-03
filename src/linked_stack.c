/*
 * Stack (LIFO) built on a singly linked list.
 *
 * Unlike the array version this stack has no fixed capacity: pushing only
 * fails if malloc fails.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Stack {
    struct Node *top;
    int count;
};

void stack_init(struct Stack *s) {
    s->top = NULL;
    s->count = 0;
}

int stack_is_empty(const struct Stack *s) {
    return s->count == 0;
}

int stack_size(const struct Stack *s) {
    return s->count;
}

/* Pushes a value. Returns 1 on success, 0 if the allocation failed. */
int stack_push(struct Stack *s, int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("stack_push: out of memory\n");
        return 0;
    }
    node->data = data;
    node->next = s->top;
    s->top = node;
    s->count++;
    return 1;
}

/* Removes the top value into *out. Returns 1 on success, 0 if empty. */
int stack_pop(struct Stack *s, int *out) {
    if (stack_is_empty(s)) {
        printf("stack_pop: stack is empty\n");
        return 0;
    }
    struct Node *doomed = s->top;
    *out = doomed->data;
    s->top = doomed->next;
    free(doomed);
    s->count--;
    return 1;
}

/* Reads the top value without removing it. Returns 1 on success, 0 if empty. */
int stack_peek(const struct Stack *s, int *out) {
    if (stack_is_empty(s)) {
        printf("stack_peek: stack is empty\n");
        return 0;
    }
    *out = s->top->data;
    return 1;
}

/* Prints the stack from top to bottom. */
void stack_print(const struct Stack *s) {
    if (stack_is_empty(s)) {
        printf("(empty)\n");
        return;
    }
    struct Node *current = s->top;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("(bottom)\n");
}

/* Frees every node and leaves the stack usable. */
void stack_clear(struct Stack *s) {
    int discarded;
    while (!stack_is_empty(s)) {
        stack_pop(s, &discarded);
    }
}

int main(void) {
    struct Stack s;
    int value = 0;

    printf("== Linked list stack ==\n");

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
    assert(s.top == NULL);
    assert(stack_pop(&s, &value) == 0);

    /* The list based stack grows past any array limit. */
    for (int i = 0; i < 1000; i++) {
        assert(stack_push(&s, i) == 1);
    }
    assert(stack_size(&s) == 1000);
    assert(stack_peek(&s, &value) == 1 && value == 999);

    stack_clear(&s);
    assert(stack_is_empty(&s));
    assert(s.top == NULL);
    printf("after clear           : ");
    stack_print(&s);

    /* The stack is still usable after being cleared. */
    stack_push(&s, 7);
    assert(stack_peek(&s, &value) == 1 && value == 7);
    stack_clear(&s);

    printf("All linked list stack tests passed.\n");
    return 0;
}
