#include "basic.h"
#include "action.h"

int main() {
    srand(time(NULL));
    int choice, i;
    menu(start, false);
    while (round_cnt < 9 && mr_jack.end_of_game == false) {
        if (round_cnt % 2 == 1) {
            if (prev_game==false){creat_lists();}
            if (turn==5){ turn=1;}
            for (turn; turn < 5; turn++) {
                arrest=false;
                if (turn == 1 || turn == 4) {
                    player = detective;
                    printf("\n detective's turn: do you want to arrest anyone?\n 0)main menu\n 1)yes  2)no\n");
                    scanf("%d", &choice);
                    while (choice==0) {
                        system("cls");
                        menu(during, false);
                        printf("\n detective's turn: do you want to arrest anyone?\n 0)main menu\n 1)yes  2)no\n");
                        scanf("%d", &choice);
                    }
                    if (choice == 1) { arrest = true; }
                    printf("\n choose a character:\n");
                } else {
                    player = Jack;
                    printf("\n Jack's turn\n choose a character:\n");
                }
                print_actions(this_round);
                scanf("%d", &choice);
                while (choice==0) {
                    system("cls");
                    menu(during, true);
                    if (mr_jack.end_of_game==true)
                        return 0;
                    else{
                        print_actions(this_round);
                        scanf("%d", &choice);
                    }
                }
                sleep(1);
                system("cls");
                print_map();
                act(choice, this_round);
                if (mr_jack.end_of_game == true)
                    return 0;
                save_graphic_map();
                remove_node(round_cnt, choice);
            }
            call_for_witness();
            prev_game=false;
        } else {
            if (turn==5){ turn=1; }
            for (turn ; turn < 5; turn++) {
                arrest=false;
                if (turn == 2 || turn == 3) {
                    player = detective;
                    printf("\n detective's turn: do you want to arrest anyone?\n 0)main menu\n 1)yes  2)no\n");
                    scanf("%d", &choice);
                    while (choice==0) {
                        system("cls");
                        menu(during, false);
                        printf("\n detective's turn: do you want to arrest anyone?\n 0)main menu\n 1)yes  2)no\n");
                        scanf("%d", &choice);
                    }
                    if (choice == 1) { arrest = true; }
                    printf(" choose a character:\n");
                }
                else {
                    player = Jack;
                    printf("\n Jack's turn\n choose a character:\n");
                }
                print_actions(next_round);
                scanf("%d", &choice);
                while (choice==0) {
                    system("cls");
                    menu(during, true);
                    if (mr_jack.end_of_game==true)
                        return 0;
                    else{
                        print_actions(next_round);
                        scanf("%d", &choice);
                    }
                }
                sleep(1);
                system("cls");
                print_map();
                act(choice, next_round);
                if (mr_jack.end_of_game == true)
                    return 0;
                save_graphic_map();
                remove_node(round_cnt, choice);
            }
            call_for_witness();
            free(list);
        }
        if (round_cnt < 5) {
            remove_lamp(round_cnt);
        }
        round_cnt++;
    }
    if (round_cnt == 8) {
        printf2(12, "%s","\n Mr.Jack won the game\n")
        sleep(3);
    }
    return 0;
}
