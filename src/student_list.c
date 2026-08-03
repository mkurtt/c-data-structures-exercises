/*
 * Singly linked list holding student records.
 *
 * The original version read every field with scanf, which makes the code
 * impossible to test automatically. Here the data comes in as arguments and
 * the interactive input lives in read_student_from_stdin(), which the demo
 * does not call.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN  30
#define CLASS_LEN 10

struct Student {
    char first_name[NAME_LEN];
    char last_name[NAME_LEN];
    int number;
    char class_name[CLASS_LEN];
    struct Student *next;
};

/* Copies src into dst and always terminates the string. */
static void copy_field(char *dst, size_t size, const char *src) {
    strncpy(dst, src, size - 1);
    dst[size - 1] = '\0';
}

/* Inserts a student at the front of the list and returns the new head. */
struct Student *add_student(struct Student *head, const char *first_name,
                            const char *last_name, int number,
                            const char *class_name) {
    struct Student *student = (struct Student *)malloc(sizeof(struct Student));
    if (student == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return head;
    }
    copy_field(student->first_name, NAME_LEN, first_name);
    copy_field(student->last_name, NAME_LEN, last_name);
    copy_field(student->class_name, CLASS_LEN, class_name);
    student->number = number;
    student->next = head;
    return student;
}

/* Reads one student from stdin and adds it. Kept out of the demo on purpose. */
struct Student *read_student_from_stdin(struct Student *head) {
    char first_name[NAME_LEN];
    char last_name[NAME_LEN];
    char class_name[CLASS_LEN];
    int number;

    printf("First name: ");
    if (scanf("%29s", first_name) != 1) return head;
    printf("Last name: ");
    if (scanf("%29s", last_name) != 1) return head;
    printf("Number: ");
    if (scanf("%d", &number) != 1) return head;
    printf("Class: ");
    if (scanf("%9s", class_name) != 1) return head;

    return add_student(head, first_name, last_name, number, class_name);
}

void print_students(struct Student *head) {
    if (head == NULL) {
        printf("(no students)\n");
        return;
    }
    printf("%-12s %-12s %8s %-8s\n", "FIRST NAME", "LAST NAME", "NUMBER", "CLASS");
    struct Student *current = head;
    while (current != NULL) {
        printf("%-12s %-12s %8d %-8s\n", current->first_name, current->last_name,
               current->number, current->class_name);
        current = current->next;
    }
}

/* Returns the student with the given number, or NULL. */
struct Student *find_student(struct Student *head, int number) {
    while (head != NULL) {
        if (head->number == number) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

struct Student *remove_student(struct Student *head, int number) {
    if (head == NULL) {
        printf("remove_student: list is empty\n");
        return NULL;
    }
    if (head->number == number) {
        struct Student *doomed = head;
        head = head->next;
        free(doomed);
        return head;
    }
    struct Student *current = head;
    while (current->next != NULL && current->next->number != number) {
        current = current->next;
    }
    if (current->next == NULL) {
        printf("remove_student: number %d not found\n", number);
        return head;
    }
    struct Student *doomed = current->next;
    current->next = doomed->next;
    free(doomed);
    return head;
}

int student_count(struct Student *head) {
    int count = 0;
    while (head != NULL) {
        head = head->next;
        count++;
    }
    return count;
}

struct Student *free_students(struct Student *head) {
    while (head != NULL) {
        struct Student *doomed = head;
        head = head->next;
        free(doomed);
    }
    return NULL;
}

int main(void) {
    struct Student *head = NULL;

    printf("== Student list ==\n");

    assert(student_count(head) == 0);
    print_students(head); /* must not crash while empty */

    head = add_student(head, "Ada", "Lovelace", 101, "1-A");
    head = add_student(head, "Alan", "Turing", 102, "1-A");
    head = add_student(head, "Grace", "Hopper", 103, "1-B");
    print_students(head);
    assert(student_count(head) == 3);

    struct Student *found = find_student(head, 102);
    assert(found != NULL);
    assert(strcmp(found->last_name, "Turing") == 0);
    printf("found 102: %s %s (%s)\n", found->first_name, found->last_name,
           found->class_name);

    assert(find_student(head, 999) == NULL); /* missing number returns NULL */

    head = remove_student(head, 103); /* head */
    head = remove_student(head, 101); /* tail */
    printf("after removing 103 and 101:\n");
    print_students(head);
    assert(student_count(head) == 1);
    assert(find_student(head, 103) == NULL);
    assert(find_student(head, 101) == NULL);
    assert(find_student(head, 102) != NULL);

    head = remove_student(head, 777); /* missing number must not crash */
    assert(student_count(head) == 1);

    head = free_students(head);
    assert(head == NULL);
    assert(student_count(head) == 0);

    head = remove_student(head, 1); /* removing from an empty list is safe */
    assert(head == NULL);

    printf("All student list tests passed.\n");
    return 0;
}
