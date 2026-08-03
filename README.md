# C Data Structures

Classic data structures implemented from scratch in C, one self-contained program
per structure. Every program has a `main()` that demonstrates the operations and
then verifies them with `assert`, so running a program is the same as testing it.

No external dependencies — a C11 compiler and `make` are all you need.

## Contents

| Program | Structure | Notes |
| --- | --- | --- |
| [`src/singly_linked_list.c`](src/singly_linked_list.c) | Singly linked list | Insert at front/back/after a value, search, update, delete, length, node addresses |
| [`src/doubly_linked_list.c`](src/doubly_linked_list.c) | Doubly linked list | Forward and backward traversal, plus a link-consistency check |
| [`src/circular_linked_list.c`](src/circular_linked_list.c) | Circular singly linked list | Last node points back to the head; no `NULL` terminator |
| [`src/student_list.c`](src/student_list.c) | Singly linked list of records | Student records (name, number, class) instead of plain integers |
| [`src/array_stack.c`](src/array_stack.c) | Stack (LIFO), array | Fixed capacity, with an overflow guard |
| [`src/linked_stack.c`](src/linked_stack.c) | Stack (LIFO), linked list | Grows until memory runs out |
| [`src/circular_queue.c`](src/circular_queue.c) | Queue (FIFO), array | Ring buffer: `front` and `rear` wrap around so freed slots get reused |
| [`src/linked_queue.c`](src/linked_queue.c) | Queue (FIFO), linked list | Keeps a `rear` pointer so `enqueue` is O(1) |

## Build and run

```sh
make          # compile every program into bin/
make test     # compile, then run all of them and report pass/fail
make clean    # remove bin/
```

Run a single program directly:

```sh
make
./bin/circular_queue
```

Sample output:

```
== Circular array queue (capacity 5) ==
(empty)
queue_dequeue: queue is empty
queue_peek: queue is empty
after enqueue 10..40  : 10 -> 20 -> 30 -> 40 -> NULL
after one dequeue     : 20 -> 30 -> 40 -> NULL
after enqueue 50, 60  : 20 -> 30 -> 40 -> 50 -> 60 -> NULL
queue_enqueue: queue is full
after reset           : (empty)
All circular queue tests passed.
```

## Conventions

A few rules are applied consistently across all eight programs:

- **Functions that can change the first element return the new head.** For a
  linked list, inserting at the front or deleting the first node changes which
  node the head is. Returning the head means an empty list is handled correctly
  by the caller: `head = push_front(head, 10);`
- **Read operations report success separately from the value.** `pop`, `dequeue`
  and `peek` return `1` on success and `0` when the container is empty, and write
  the value to an out parameter:

  ```c
  int value;
  if (stack_pop(&s, &value)) {
      /* value is valid here */
  }
  ```

  Returning `-1` to mean "empty" would be ambiguous, because `-1` is also a
  perfectly valid value to store.
- **Every operation is safe on an empty container.** Printing, deleting or
  searching an empty list prints a message and returns instead of dereferencing
  `NULL`.
- **Everything that is allocated is freed.** Each program has a `free_list` /
  `clear` function, and the tests call it.

## Tests

The tests are `assert` calls inside each `main()`. Beyond the happy path they
cover the cases that are easy to get wrong:

- deleting the first, last, and a middle element
- deleting a value that is not in the container
- every operation on an empty container
- filling a fixed-capacity stack or queue and pushing one item too many
- a circular queue whose `rear` index wraps past the end of the array
- a linked queue that drains completely, so the `rear` pointer must not dangle
- a doubly linked list where every `next` link has a matching `prev` link

`make test` builds everything and runs it:

```
PASS  array_stack
PASS  circular_linked_list
PASS  circular_queue
PASS  doubly_linked_list
PASS  linked_queue
PASS  linked_stack
PASS  singly_linked_list
PASS  student_list

All 8 programs passed.
```

## License

MIT
