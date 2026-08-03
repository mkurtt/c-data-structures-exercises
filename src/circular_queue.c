/*
 * Queue (FIFO) backed by a fixed size array used as a ring buffer.
 *
 * front is the index of the next item to leave, rear the index of the last
 * item that entered. Both wrap around to 0 once they pass the end of the
 * array, so the freed slots at the front are reused instead of being lost.
 * count tells a full queue apart from an empty one.
 */

#include <assert.h>
#include <stdio.h>

#define QUEUE_CAPACITY 5

struct Queue {
    int data[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int queue_is_empty(const struct Queue *q) {
    return q->count == 0;
}

int queue_is_full(const struct Queue *q) {
    return q->count == QUEUE_CAPACITY;
}

int queue_size(const struct Queue *q) {
    return q->count;
}

/* Adds a value at the rear. Returns 1 on success, 0 if the queue is full. */
int queue_enqueue(struct Queue *q, int data) {
    if (queue_is_full(q)) {
        printf("queue_enqueue: queue is full\n");
        return 0;
    }
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
    q->data[q->rear] = data;
    q->count++;
    return 1;
}

/* Removes the front value into *out. Returns 1 on success, 0 if empty. */
int queue_dequeue(struct Queue *q, int *out) {
    if (queue_is_empty(q)) {
        printf("queue_dequeue: queue is empty\n");
        return 0;
    }
    *out = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->count--;
    return 1;
}

/* Reads the front value without removing it. Returns 1 on success, 0 if empty. */
int queue_peek(const struct Queue *q, int *out) {
    if (queue_is_empty(q)) {
        printf("queue_peek: queue is empty\n");
        return 0;
    }
    *out = q->data[q->front];
    return 1;
}

/* Prints the queue from front to rear. */
void queue_print(const struct Queue *q) {
    if (queue_is_empty(q)) {
        printf("(empty)\n");
        return;
    }
    int index = q->front;
    for (int i = 0; i < q->count; i++) {
        printf("%d -> ", q->data[index]);
        index = (index + 1) % QUEUE_CAPACITY;
    }
    printf("NULL\n");
}

void queue_reset(struct Queue *q) {
    queue_init(q);
}

int main(void) {
    struct Queue q;
    int value = 0;

    printf("== Circular array queue (capacity %d) ==\n", QUEUE_CAPACITY);

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

    assert(queue_peek(&q, &value) == 1);
    assert(value == 10);
    assert(queue_size(&q) == 4); /* peek does not remove */

    assert(queue_dequeue(&q, &value) == 1);
    assert(value == 10); /* first in, first out */
    printf("after one dequeue     : ");
    queue_print(&q);
    assert(queue_size(&q) == 3);

    /* Two more enqueues wrap the rear index back to the start of the array. */
    queue_enqueue(&q, 50);
    queue_enqueue(&q, 60);
    printf("after enqueue 50, 60  : ");
    queue_print(&q);
    assert(queue_is_full(&q));
    assert(queue_size(&q) == QUEUE_CAPACITY);
    assert(q.rear < q.front); /* the rear really did wrap around */

    assert(queue_enqueue(&q, 70) == 0); /* one enqueue too many */
    assert(queue_size(&q) == QUEUE_CAPACITY);

    /* The values must still come out in insertion order across the wrap. */
    int expected[] = {20, 30, 40, 50, 60};
    for (int i = 0; i < 5; i++) {
        assert(queue_dequeue(&q, &value) == 1);
        assert(value == expected[i]);
    }
    assert(queue_is_empty(&q));
    assert(queue_dequeue(&q, &value) == 0);

    queue_enqueue(&q, 1);
    queue_reset(&q);
    assert(queue_is_empty(&q));
    assert(queue_size(&q) == 0);
    printf("after reset           : ");
    queue_print(&q);

    printf("All circular queue tests passed.\n");
    return 0;
}
