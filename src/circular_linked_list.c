/*
 * Circular singly linked list of integers.
 *
 * The last node points back to the head, so the list has no NULL terminator.
 * Every traversal must therefore stop when it reaches the head again.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

static struct Node *create_node(int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = node; /* a single node points to itself */
    return node;
}

/* Returns the last node, i.e. the one that links back to the head. */
static struct Node *last_node(struct Node *head) {
    if (head == NULL) {
        return NULL;
    }
    struct Node *last = head;
    while (last->next != head) {
        last = last->next;
    }
    return last;
}

/* Inserts a new node and makes it the new head. */
struct Node *push_front(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (head == NULL) {
        return node;
    }
    struct Node *last = last_node(head);
    node->next = head;
    last->next = node;
    return node;
}

/* Appends a new node at the end; the head does not change unless it was empty. */
struct Node *push_back(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (head == NULL) {
        return node;
    }
    struct Node *last = last_node(head);
    node->next = head;
    last->next = node;
    return head;
}

struct Node *find(struct Node *head, int value) {
    if (head == NULL) {
        return NULL;
    }
    struct Node *current = head;
    do {
        if (current->data == value) {
            return current;
        }
        current = current->next;
    } while (current != head);
    return NULL;
}

struct Node *remove_value(struct Node *head, int value) {
    if (head == NULL) {
        printf("remove_value: list is empty\n");
        return NULL;
    }
    if (head->data == value) {
        if (head->next == head) { /* the only node */
            free(head);
            return NULL;
        }
        struct Node *last = last_node(head);
        struct Node *doomed = head;
        head = head->next;
        last->next = head;
        free(doomed);
        return head;
    }
    struct Node *current = head;
    while (current->next != head && current->next->data != value) {
        current = current->next;
    }
    if (current->next == head) {
        printf("remove_value: value %d not found\n", value);
        return head;
    }
    struct Node *doomed = current->next;
    current->next = doomed->next;
    free(doomed);
    return head;
}

int length(struct Node *head) {
    if (head == NULL) {
        return 0;
    }
    int count = 0;
    struct Node *current = head;
    do {
        current = current->next;
        count++;
    } while (current != head);
    return count;
}

void print_list(struct Node *head) {
    if (head == NULL) {
        printf("(empty)\n");
        return;
    }
    struct Node *current = head;
    do {
        printf("%d -> ", current->data);
        current = current->next;
    } while (current != head);
    printf("(back to %d)\n", head->data);
}

struct Node *free_list(struct Node *head) {
    if (head == NULL) {
        return NULL;
    }
    struct Node *current = head->next;
    while (current != head) {
        struct Node *doomed = current;
        current = current->next;
        free(doomed);
    }
    free(head);
    return NULL;
}

int main(void) {
    struct Node *head = NULL;

    printf("== Circular linked list ==\n");

    assert(length(head) == 0);
    print_list(head); /* must not crash while empty */

    head = push_front(head, 10);
    assert(head->next == head); /* single node links to itself */
    head = push_front(head, 20);
    head = push_front(head, 30);
    printf("after push_front 10, 20, 30 : ");
    print_list(head);
    assert(length(head) == 3);
    assert(head->data == 30);

    head = push_back(head, 40);
    head = push_back(head, 50);
    printf("after push_back 40, 50      : ");
    print_list(head);
    assert(length(head) == 5);
    assert(head->data == 30);
    assert(last_node(head)->data == 50);
    assert(last_node(head)->next == head);

    assert(find(head, 40) != NULL);
    assert(find(head, 999) == NULL);

    head = remove_value(head, 30); /* head */
    printf("after remove 30 (head)      : ");
    print_list(head);
    assert(head->data == 20);
    assert(last_node(head)->next == head);

    head = remove_value(head, 50); /* tail */
    head = remove_value(head, 10); /* middle */
    printf("after remove 50, 10         : ");
    print_list(head);
    assert(length(head) == 2);
    assert(last_node(head)->next == head);

    head = remove_value(head, 777); /* missing value must not crash */
    assert(length(head) == 2);

    head = remove_value(head, 20);
    head = remove_value(head, 40);
    assert(head == NULL); /* removing the last node empties the list */
    printf("after removing everything   : ");
    print_list(head);

    head = push_front(head, 1);
    head = push_back(head, 2);
    head = free_list(head);
    assert(head == NULL);
    assert(length(head) == 0);

    printf("All circular linked list tests passed.\n");
    return 0;
}
