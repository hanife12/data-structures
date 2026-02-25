#include <stdio.h>
#include "linked_list.h"

int main() {
    Node* head = NULL;

    push_front(&head, 3);
    push_front(&head, 1);
    push_back(&head, 5);
    push_back(&head, 7);

    print_list(head);

    free_list(head);
    return 0;
}