/*
 * Queue (FIFO) built on a singly linked list.
 *
 * front points at the node that leaves next, rear at the last node that
 * entered. Keeping a rear pointer makes enqueue O(1) instead of O(n).
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Queue {
    struct Node *front;
    struct Node *rear;
    int count;
};

void queue_init(struct Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

int queue_is_empty(const struct Queue *q) {
    return q->count == 0;
}

int queue_size(const struct Queue *q) {
    return q->count;
}

/* Adds a value at the rear. Returns 1 on success, 0 if the allocation failed. */
int queue_enqueue(struct Queue *q, int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("queue_enqueue: out of memory\n");
        return 0;
    }
    node->data = data;
    node->next = NULL;

    if (queue_is_empty(q)) {
        q->front = node;
    } else {
        q->rear->next = node;
    }
    q->rear = node;
    q->count++;
    return 1;
}

/* Removes the front value into *out. Returns 1 on success, 0 if empty. */
int queue_dequeue(struct Queue *q, int *out) {
    if (queue_is_empty(q)) {
        printf("queue_dequeue: queue is empty\n");
        return 0;
    }
    struct Node *doomed = q->front;
    *out = doomed->data;
    q->front = doomed->next;
    if (q->front == NULL) { /* the queue just became empty */
        q->rear = NULL;
    }
    free(doomed);
    q->count--;
    return 1;
}

/* Reads the front value without removing it. Returns 1 on success, 0 if empty. */
int queue_peek(const struct Queue *q, int *out) {
    if (queue_is_empty(q)) {
        printf("queue_peek: queue is empty\n");
        return 0;
    }
    *out = q->front->data;
    return 1;
}

/* Prints the queue from front to rear. */
void queue_print(const struct Queue *q) {
    if (queue_is_empty(q)) {
        printf("(empty)\n");
        return;
    }
    struct Node *current = q->front;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

/* Frees every node and leaves the queue usable. */
void queue_clear(struct Queue *q) {
    int discarded;
    while (!queue_is_empty(q)) {
        queue_dequeue(q, &discarded);
    }
}

int main(void) {
    struct Queue q;
    int value = 0;

    printf("== Linked list queue ==\n");

    queue_init(&q);
    assert(queue_is_empty(&q));
    assert(queue_size(&q) == 0);
    queue_print(&q); /* must not crash while empty */

    assert(queue_dequeue(&q, &value) == 0); /* dequeue on empty fails cleanly */
    assert(queue_peek(&q, &value) == 0);

    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_enqueue(&q, 40);
    printf("after enqueue 10..40  : ");
    queue_print(&q);
    assert(queue_size(&q) == 4);
    assert(q.front->data == 10);
    assert(q.rear->data == 40);

    assert(queue_peek(&q, &value) == 1);
    assert(value == 10);
    assert(queue_size(&q) == 4); /* peek does not remove */

    assert(queue_dequeue(&q, &value) == 1);
    assert(value == 10); /* first in, first out */
    printf("after one dequeue     : ");
    queue_print(&q);
    assert(queue_size(&q) == 3);

    int expected[] = {20, 30, 40};
    for (int i = 0; i < 3; i++) {
        assert(queue_dequeue(&q, &value) == 1);
        assert(value == expected[i]);
    }
    assert(queue_is_empty(&q));
    assert(q.front == NULL);
    assert(q.rear == NULL); /* the rear pointer must not dangle */
    assert(queue_dequeue(&q, &value) == 0);

    /* Reusing the queue after it drained must still work. */
    queue_enqueue(&q, 99);
    assert(queue_size(&q) == 1);
    assert(q.front == q.rear);
    assert(queue_peek(&q, &value) == 1 && value == 99);

    for (int i = 0; i < 1000; i++) {
        assert(queue_enqueue(&q, i) == 1);
    }
    assert(queue_size(&q) == 1001);

    queue_clear(&q);
    assert(queue_is_empty(&q));
    assert(q.front == NULL && q.rear == NULL);
    printf("after clear           : ");
    queue_print(&q);

    printf("All linked list queue tests passed.\n");
    return 0;
}
