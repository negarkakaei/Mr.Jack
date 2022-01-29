#include "list.h"

// این تابع list را تشکیل داده و شافل می کند.
void shuffle_cards() {
    int type, i;
    list = (struct node *) malloc(sizeof(struct node));
    struct node *head = list;
    for (i = 1; i < 8; i++) {
        head->type = i;
        head->next = (struct node *) malloc(sizeof(struct node));
        head = head->next;
    }
    head->type = 8;
    head->next = NULL;
    head = list;
    struct node *tmp = list;
    while (head != NULL) {
        type = rand() % 8 + 1;
        while (tmp->type != type) {
            tmp = tmp->next;
        }
        type = head->type;
        head->type = tmp->type;
        tmp->type = type;
        head = head->next;
        tmp = list;
    }
}

//  این تابع لینک لیست this_round و next_round را تشکیل می دهد.
void creat_lists() {
    int type, i;
    shuffle_cards();
    this_round = list;
    next_round = list->next->next->next->next;
    this_round->next->next->next->next = NULL;

}

//این تابع پس از انتخاب هر شخصیت در هر دور بازی ان را از لینک لیست اکشن های موجود حذف می کند.
void remove_node(int round, int choice) {
    struct node *previous, *current;
    int cnt = 2;
    if (round % 2 == 1) {
        previous = this_round;
        current = this_round->next;
        if (choice == 1) {
            this_round = current;
            return;
        }
    } else {
        previous = next_round;
        current = next_round->next;
        if (choice == 1) {
            next_round = current;
            return;
        }
    }
    while (cnt <= choice) {
        if (cnt == choice) {
            previous->next = current->next;
            free(current);
            return;
        } else {
            previous = current;
            current = current->next;
            cnt++;
        }
    }

}
