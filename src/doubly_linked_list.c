/*
 * Doubly linked list of integers.
 *
 * Each node knows both its successor and its predecessor, so the list can be
 * walked in both directions.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
};

static struct Node *create_node(int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

struct Node *push_front(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (head != NULL) {
        node->next = head;
        head->prev = node;
    }
    return node;
}

struct Node *push_back(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (head == NULL) {
        return node;
    }
    struct Node *last = head;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = node;
    node->prev = last;
    return head;
}

struct Node *find(struct Node *head, int value) {
    while (head != NULL) {
        if (head->data == value) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/* Inserts data right after the first node holding target. Returns 1 on success. */
int insert_after(struct Node *head, int target, int data) {
    struct Node *current = find(head, target);
    if (current == NULL) {
        printf("insert_after: value %d not found\n", target);
        return 0;
    }
    struct Node *node = create_node(data);
    node->next = current->next;
    node->prev = current;
    if (current->next != NULL) {
        current->next->prev = node;
    }
    current->next = node;
    return 1;
}

struct Node *remove_value(struct Node *head, int value) {
    if (head == NULL) {
        printf("remove_value: list is empty\n");
        return NULL;
    }
    struct Node *doomed = find(head, value);
    if (doomed == NULL) {
        printf("remove_value: value %d not found\n", value);
        return head;
    }
    if (doomed->prev != NULL) {
        doomed->prev->next = doomed->next;
    } else {
        head = doomed->next;
    }
    if (doomed->next != NULL) {
        doomed->next->prev = doomed->prev;
    }
    free(doomed);
    return head;
}

int length(struct Node *head) {
    int count = 0;
    while (head != NULL) {
        head = head->next;
        count++;
    }
    return count;
}

void print_forward(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void print_backward(struct Node *head) {
    if (head == NULL) {
        printf("NULL\n");
        return;
    }
    struct Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->prev;
    }
    printf("NULL\n");
}

struct Node *free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *doomed = head;
        head = head->next;
        free(doomed);
    }
    return NULL;
}

/* Checks that every forward link has a matching backward link. */
static int links_are_consistent(struct Node *head) {
    if (head != NULL && head->prev != NULL) {
        return 0;
    }
    while (head != NULL && head->next != NULL) {
        if (head->next->prev != head) {
            return 0;
        }
        head = head->next;
    }
    return 1;
}

int main(void) {
    struct Node *head = NULL;

    printf("== Doubly linked list ==\n");

    head = push_front(head, 10);
    head = push_front(head, 20);
    head = push_front(head, 30);
    head = push_front(head, 40);
    printf("after push_front 10..40    : ");
    print_forward(head);
    assert(length(head) == 4);
    assert(head->data == 40);
    assert(head->prev == NULL);

    head = push_back(head, 60);
    printf("after push_back 60         : ");
    print_forward(head);
    assert(length(head) == 5);
    assert(links_are_consistent(head));

    assert(insert_after(head, 10, 80) == 1);
    printf("after insert_after(10, 80) : ");
    print_forward(head);
    printf("backward                   : ");
    print_backward(head);
    assert(length(head) == 6);
    assert(links_are_consistent(head));
    assert(find(head, 80)->prev->data == 10);
    assert(find(head, 80)->next->data == 60);

    assert(insert_after(head, 999, 1) == 0); /* missing target */

    head = remove_value(head, 40); /* head */
    head = remove_value(head, 60); /* tail */
    head = remove_value(head, 80); /* middle */
    printf("after removing 40, 60, 80  : ");
    print_forward(head);
    printf("backward                   : ");
    print_backward(head);
    assert(length(head) == 3);
    assert(links_are_consistent(head));
    assert(head->data == 30);

    head = remove_value(head, 777); /* missing value must not crash */
    assert(length(head) == 3);

    head = free_list(head);
    assert(head == NULL);
    printf("after free_list            : ");
    print_forward(head);
    print_backward(head); /* must not crash on an empty list */

    printf("All doubly linked list tests passed.\n");
    return 0;
}
