/*
 * Singly linked list of integers.
 *
 * Every function that can change the first element returns the new head,
 * so an empty list is always handled correctly by the caller.
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
    node->next = NULL;
    return node;
}

/* Inserts a new node at the front of the list. */
struct Node *push_front(struct Node *head, int data) {
    struct Node *node = create_node(data);
    node->next = head;
    return node;
}

/* Appends a new node at the end of the list. */
struct Node *push_back(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (head == NULL) {
        return node;
    }
    struct Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    return head;
}

/* Returns the first node holding the given value, or NULL. */
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
    current->next = node;
    return 1;
}

/* Replaces old_value with new_value. Returns 1 on success. */
int update_value(struct Node *head, int old_value, int new_value) {
    struct Node *current = find(head, old_value);
    if (current == NULL) {
        printf("update_value: value %d not found\n", old_value);
        return 0;
    }
    current->data = new_value;
    return 1;
}

/* Removes the first node holding the given value. */
struct Node *remove_value(struct Node *head, int value) {
    if (head == NULL) {
        printf("remove_value: list is empty\n");
        return NULL;
    }
    if (head->data == value) {
        struct Node *doomed = head;
        head = head->next;
        free(doomed);
        return head;
    }
    struct Node *current = head;
    while (current->next != NULL && current->next->data != value) {
        current = current->next;
    }
    if (current->next == NULL) {
        printf("remove_value: value %d not found\n", value);
        return head;
    }
    struct Node *doomed = current->next;
    current->next = doomed->next;
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

void print_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

/* Prints the address, value and link of every node. */
void print_details(struct Node *head) {
    int index = 1;
    while (head != NULL) {
        printf("node %d | address: %p | value: %d | next: %p\n",
               index, (void *)head, head->data, (void *)head->next);
        index++;
        head = head->next;
    }
}

/* Frees every node and returns NULL so the caller cannot keep a dangling head. */
struct Node *free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *doomed = head;
        head = head->next;
        free(doomed);
    }
    return NULL;
}

int main(void) {
    struct Node *head = NULL;

    printf("== Singly linked list ==\n");

    head = push_front(head, 10);
    head = push_front(head, 20);
    head = push_front(head, 30);
    printf("after push_front 10, 20, 30 : ");
    print_list(head);
    assert(length(head) == 3);
    assert(head->data == 30);

    head = push_back(head, 40);
    head = push_back(head, 50);
    printf("after push_back 40, 50     : ");
    print_list(head);
    assert(length(head) == 5);

    assert(insert_after(head, 20, 25) == 1);
    printf("after insert_after(20, 25) : ");
    print_list(head);
    assert(find(head, 25) != NULL);
    assert(find(head, 25)->next->data == 10);

    assert(insert_after(head, 999, 1) == 0); /* missing target */

    assert(update_value(head, 25, 26) == 1);
    printf("after update_value(25, 26) : ");
    print_list(head);
    assert(find(head, 25) == NULL);
    assert(find(head, 26) != NULL);

    head = remove_value(head, 30); /* first element */
    head = remove_value(head, 50); /* last element  */
    head = remove_value(head, 10); /* middle element */
    printf("after removing 30, 50, 10  : ");
    print_list(head);
    assert(length(head) == 3); /* 20, 26, 40 are left */
    assert(find(head, 30) == NULL);
    assert(find(head, 50) == NULL);
    assert(find(head, 10) == NULL);

    head = remove_value(head, 777); /* missing value must not crash */
    assert(length(head) == 3);

    printf("node details:\n");
    print_details(head);

    head = free_list(head);
    assert(head == NULL);
    assert(length(head) == 0);
    printf("after free_list            : ");
    print_list(head);

    /* Removing from an empty list must be safe. */
    head = remove_value(head, 1);
    assert(head == NULL);

    printf("All singly linked list tests passed.\n");
    return 0;
}
