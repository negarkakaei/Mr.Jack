#include "action.h"

//این تابع مختصات اولیه و جهت حرکت را گرفته و مختصات ثانویه را پیدا میکند.
void find_coordinates(int current_i, int current_j, int *next_i, int *next_j, enum direction choice) {
    if (choice == up) {
        *next_i = current_i - 1;
        *next_j = current_j;
    } else if (choice == down) {
        *next_i = current_i + 1;
        *next_j = current_j;
    } else {
        if (current_j % 2 == 0) {
            if (choice == up_right) {
                *next_i = current_i;
                *next_j = current_j + 1;
            } else if (choice == up_left) {
                *next_i = current_i;
                *next_j = current_j - 1;
            } else if (choice == down_right) {
                *next_i = current_i + 1;
                *next_j = current_j + 1;
            } else if (choice == down_left) {
                *next_i = current_i + 1;
                *next_j = current_j - 1;
            }
        } else {
            if (choice == up_right) {
                *next_i = current_i - 1;
                *next_j = current_j + 1;
            } else if (choice == up_left) {
                *next_i = current_i - 1;
                *next_j = current_j - 1;
            } else if (choice == down_right) {
                *next_i = current_i;
                *next_j = current_j + 1;
            } else if (choice == down_left) {
                *next_i = current_i;
                *next_j = current_j - 1;
            }
        }
    }
}

// این تابع شخصیت ها و اشیاء را بین خونه ها جابه جا می کند و تغییرات لازم را در مپ منطقی و گرافیکی اعمال می کند
void update(int current_i, int current_j, int next_i, int next_j, char object[10]) {
    char next_symbol[10];
    char current_symbol[10];
    char n[2];
    n[1] = '\0';
    int number, i;
    if (strcmp(object, "exit") == 0) {
        logical_map[current_i][current_j].type=exit_on;
        logical_map[next_i][next_j].type=exit_off;

        number = logical_map[current_i][current_j].num;
        logical_map[current_i][current_j].num = logical_map[next_i][next_j].num;
        logical_map[next_i][next_j].num = number;

        n[0] = logical_map[current_i][current_j].num + 48;
        strcpy(current_symbol, "E");
        strcat(current_symbol, n);

        n[0] = logical_map[next_i][next_j].num + 48;
        strcpy(next_symbol, "B");
        strcat(next_symbol, n);

    } else if (strcmp(object, "lamp") == 0) {
        logical_map[current_i][current_j].type = lamp_off;
        logical_map[next_i][next_j].type = lamp_on;

        number = logical_map[current_i][current_j].num;
        logical_map[current_i][current_j].num = logical_map[next_i][next_j].num;
        logical_map[next_i][next_j].num = number;

        n[0] = logical_map[current_i][current_j].num + 48;
        strcpy(current_symbol, "OFF ");
        strcat(current_symbol, n);

        n[0] = logical_map[next_i][next_j].num + 48;
        strcpy(next_symbol, "ON ");
        strcat(next_symbol, n);
        strcat(next_symbol, " ");

    } else if (strcmp(object, "manhole") == 0) {
        logical_map[current_i][current_j].type = open_well;
        logical_map[next_i][next_j].type = close_well;

        number = logical_map[current_i][current_j].num;
        logical_map[current_i][current_j].num = logical_map[next_i][next_j].num;
        logical_map[next_i][next_j].num = number;

        n[0] = logical_map[current_i][current_j].num + 48;
        strcpy(current_symbol, "(");
        strcat(current_symbol, n);
        strcat(current_symbol, ") ");

        n[0] = logical_map[next_i][next_j].num + 48;
        strcpy(next_symbol, "(x");
        strcat(next_symbol, n);
        strcat(next_symbol, ")");

    } else {
        char *ptr;
        if (logical_map[next_i][next_j].character == 0) {
            logical_map[next_i][next_j].character = ToEnum(object);
        }
        if (strcmp(ToString(logical_map[current_i][current_j].character), object) == 0) {
            logical_map[current_i][current_j].character = 0;
        }
        if (characters_status[ToEnum(object) - 1].innocent == true) {
            char new[10] = {'(', 'i', ')', '\0'};
            strcat(new, object);
            //پاک کردن کارکتر از خونه قبلی
            for (i = 0; i < 39; i++) {
                ptr = strstr(graphic_map[i], new);
                if (ptr != NULL)
                    break;
            }
            strncpy(ptr, "      ", 5);

            // اضافه کردن کارکتر به خونه جدید
            if (next_j % 2 == 0) {
                if (strncmp(&graphic_map[4 * next_i + 3][8 * next_j + 3], "  ", 2) != 0) {
                    strncpy(&graphic_map[4 * next_i + 5][8 * next_j + 3], new, strlen(new));
                } else {
                    strncpy(&graphic_map[4 * next_i + 3][8 * next_j + 3], new, strlen(new));
                }
            } else {
                if (strncmp(&graphic_map[4 * next_i + 1][(next_j / 2) * 16 + 11], "  ", 2) != 0) {
                    strncpy(&graphic_map[4 * next_i + 3][(next_j / 2) * 16 + 11], new, strlen(new));
                } else {
                    strncpy(&graphic_map[4 * next_i + 1][(next_j / 2) * 16 + 11], new, strlen(new));
                }
            }

        } else {
            //پاک کردن کارکتر از خونه قبلی
            for (i = 0; i < 39; i++) {
                ptr = strstr(graphic_map[i], object);
                if (ptr != NULL)
                    break;
            }
            strncpy(ptr, "  ", 2);

            // اضافه کردن کارکتر به خونه جدید
            if (next_j % 2 == 0) {
                if (strncmp(&graphic_map[4 * next_i + 3][8 * next_j + 6], "  ", 2) != 0) {
                    strncpy(&graphic_map[4 * next_i + 5][8 * next_j + 6], object, strlen(object));
                } else {
                    strncpy(&graphic_map[4 * next_i + 3][8 * next_j + 6], object, strlen(object));
                }
            } else {
                if (strncmp(&graphic_map[4 * next_i + 1][(next_j / 2) * 16 + 14], "  ", 2) != 0) {
                    strncpy(&graphic_map[4 * next_i + 3][(next_j / 2) * 16 + 14], object, strlen(object));
                } else {
                    strncpy(&graphic_map[4 * next_i + 1][(next_j / 2) * 16 + 14], object, strlen(object));
                }
            }
        }
        save_graphic_map();
        sleep(1);
        system("cls");
        print_map();
        return;
    }

    if (next_j % 2 == 0) {
        strncpy(&graphic_map[4 * next_i + 4][8 * next_j + 3], next_symbol, strlen(next_symbol));
    } else {
        strncpy(&graphic_map[4 * next_i + 2][(next_j / 2) * 16 + 11], next_symbol, strlen(next_symbol));
    }

    if (current_j % 2 == 0) {
        strncpy(& graphic_map[4 * current_i + 4][8 * current_j + 3], current_symbol, strlen(current_symbol));
    } else {
        strncpy(& graphic_map[4 * current_i + 2][(current_j / 2) * 16 + 11], current_symbol, strlen(current_symbol));
    }
    save_graphic_map();
    sleep(1);
    system("cls");
    print_map();
}

// این تابع چک میکند که ایا کاربر میتواند در جهت مد نظر حرکت کند یا نه
bool valid(enum character_type character, bool last_move, int next_i, int next_j) {
    int z;
    coordinate out_of_map[4] = {{0, 0}, {0, 12}, {8, 0},{7, 12}};
    for (z = 0; z <= p; z++) {
        if (next_i == prev_cells[z].row && next_j == prev_cells[z].column)
            return false;
    }
    for (z = 0; z < 4; z++) {
        if (next_i == out_of_map[z].row && next_j == out_of_map[z].column) {
            if (player == Jack && mr_jack.visible == false && round_cnt > 1 && character == mr_jack.character) {
                printf2(12, "%s","\n Mr.Jack won the game\n")
                sleep(3);
                mr_jack.end_of_game = true;
                return true;
            } else
                return false;
        }
    }
    if (next_i > 8 || next_i < 0 || next_j > 12 || next_j < 0)
        return false;
    if (logical_map[next_i][next_j].type == extra)
        return false;
    if (logical_map[next_i][next_j].type == house || logical_map[next_i][next_j].type == lamp_on ||
        logical_map[next_i][next_j].type == lamp_off) {
        if (character != MS || last_move == true)
            return false;
        else
            two_characters=true;
    }
    if (logical_map[next_i][next_j].character != 0) {
        if (last_move==true){
            if (arrest==true){
                if (logical_map[next_i][next_j].character == mr_jack.character) {
                    printf2(12, "%s","\n Detective won the game\n")
                    sleep(3);
                    mr_jack.end_of_game = true;
                } else {
                    printf2(12, "%s","\n Mr.Jack won the game\n")
                    sleep(3);
                    mr_jack.end_of_game = true;
                }
                return true;
            }
            else{
                return false;
            }
        }
        else {
            if (arrest==true){
                printf(" do you want to accuse this character?\n 1)yes 2)no\n");
                scanf("%d", &z);
                if (z==1) {
                    if (logical_map[next_i][next_j].character == mr_jack.character) {
                        printf2(12, "%s","\n Detective won the game\n")
                        sleep(3);
                        mr_jack.end_of_game = true;
                    } else {
                        printf2(12, "%s","\n Mr.Jack won the game\n")
                        sleep(3);
                        mr_jack.end_of_game = true;
                    }
                }
                else {
                    two_characters=true;
                    return true;
                }
            }
            else{
                two_characters=true;
                return true;
            }
        }
    }
    return true;
}

// این تابع کارکتر هارا بین خونه ها جا به جا می کند
void move(enum character_type character) {
    bool last_move = false;
    bool flag = false;
    int i, j, next_i, next_j;
    enum direction choice;
    char object[5];
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 13; j++) {
            if (logical_map[i][j].character == character) {
                prev_cells[0].row = i;
                prev_cells[0].column = j;
                flag = true;
                break;
            }
        }
        if (flag == true)
            break;
    }
    for (i = 1; (i <= 3 || (i <= 4 && character == MS)); i++) {
        flag = false;
        two_characters = false;
        if ((i == 3 && character != MS) || i == 4) {
            last_move = true;
        }
        printf("\n choose a direction\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n");
        if (logical_map[prev_cells[p].row][prev_cells[p].column].type == open_well) {
            printf("    7)jump into a manhole\n");
        }
        scanf("%d", &choice);
        while (choice==0){
            system("cls");
            menu(during, false);
            printf("\n choose a direction\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n");
            if (logical_map[prev_cells[p].row][prev_cells[p].column].type == open_well) {
                printf("    7)jump into a manhole\n");
            }
            scanf("%d", &choice);
        }
        if (choice == 7) {
            printf("\n which manhole do you want to go to?\n  0)main menu\n  1 2 3 4 5 6\n");
            scanf("%d", &choice);
            while (choice==0){
                system("cls");
                menu(during, false);
                printf("\n which manhole do you want to go to?\n  0)main menu\n  1 2 3 4 5 6\n");
                scanf("%d", &choice);
            }
            for (int row = 0; row < 9; row++) {
                for (int column = 0; column < 13; column++) {
                    if (logical_map[row][column].type == open_well && logical_map[row][column].num == choice) {
                        next_i = row;
                        next_j = column;
                        flag = true;
                        break;
                    }
                }
                if (flag == true)
                    break;
            }
        } else { find_coordinates(prev_cells[p].row, prev_cells[p].column, &next_i, &next_j, choice); }

        if (valid(character, last_move, next_i, next_j) == true) {
            if (mr_jack.end_of_game == true)
                return;
            strcpy(object, ToString(character));
            update(prev_cells[p].row, prev_cells[p].column, next_i, next_j, object);
            prev_cells[p + 1].row = next_i;
            prev_cells[p + 1].column = next_j;
            p++;
        } else {
            printf2(12, "%s","\n you can't go there!\n")
            i--;
            continue;
        }
        if (last_move == false && two_characters == false) {
            printf("\n another move?\n 1)yep 2)nope\n");
            scanf("%d", &choice);
            if (choice == 2) {
                sleep(1);
                system("cls");
                print_map();
                break;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        prev_cells[i].row = prev_cells[i].column = 13;
        p = 0;
    }
}

// این تابع به صورت رندم یکی از شخصیت های مشکوک را به بازیکن نقش شرلوک نشان داده
// سپس ان را از لیست اصلی حذف و به لیست شخصیت های قابل رویت آن بازیکن اضافه میکند.
void sherlock() {
    int random;
    char tmp[3];
    for (int i = a; i >= 0; i--) {
        random = rand() % (a + 1);
        strcpy(tmp, alibi[i]);
        strcpy(alibi[i], alibi[random]);
        strcpy(alibi[random], tmp);
    }
    if (player == detective) {
        printf2(12, "%s","\n WARNING! ONLY DETECTIVE SHOULD READ THIS!\n")
        sleep(3);
        system("cls");
        print_map();
        printf2(12, "\n your alibi card is : %s\n", alibi[a])
        sleep(3);
        system("cls");
        print_map();
        strcpy(detective_cards[d], alibi[a]);
        d++;
    } else {
        printf2(12, "%s", "\n WARNING! ONLY MR.JACK SHOULD READ THIS!\n")
        sleep(3);
        system("cls");
        print_map();
        printf2(12,"\n your alibi card is : %s\n", alibi[a])
        sleep(3);
        system("cls");
        print_map();
        strcpy(jack_cards[c], alibi[a]);
        c++;
    }
    alibi = realloc(alibi, a * sizeof(char *));
    a--;
}

// این تابع قابلیت های هر شخصیت را اجرا می کند
void ability(enum character_type character) {
    int current, next, current_i, current_j, next_i, next_j, choice=0;
    if (character == SH) {
        sherlock();
    } else if (character == JW) {
        printf("\n choose the lantern direction\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n");
        scanf("%d", &lantern_direction);
        while (lantern_direction==0){
            system("cls");
            menu(during, false);
            printf("\n choose the lantern direction\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n");
            scanf("%d", &lantern_direction);

        }
        sleep(1);
        system("cls");
        print_map();
    } else if (character == JS) {
        printf("\n which lamp do you want to turn off?\n\n 0)main menu\n");
        if (round_cnt<5){
            for (int i=round_cnt; i<=6; i++){  printf(" %d", i); }
            printf("\n");
        }
        else{ printf(" 5 6\n"); }
        scanf("%d", &current);
        while (current==0){
            system("cls");
            menu(during, false);
            printf("\n which lamp do you want to turn off?\n\n 0)main menu\n");
            if (round_cnt<5){
                for (int i=round_cnt; i<=6; i++){  printf(" %d", i); }
                printf("\n");
            }
            else{ printf(" 5 6\n"); }
            scanf("%d", &current);
        }
        printf(" which lamp do you want to turn on?\n\n 0)main menu\n");
        if (round_cnt<5){
            for (int i=1; i<=round_cnt+1; i++){ printf(" %d", i);}
            printf("\n");
        }
        else{ printf(" 1 2 3 4 5 6\n"); }
        scanf("%d", &next);
        while (next==0){
            system("cls");
            menu(during, false);
            printf(" which lamp do you want to turn on?\n\n 0)main menu\n");
            if (round_cnt<5){
                for (int i=1; i<=round_cnt+1; i++){ printf(" %d", i);}
                printf("\n");
            }
            else{ printf(" 1 2 3 4 5 6\n"); }
            scanf("%d", &next);
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 13; j++) {
                if (logical_map[i][j].type == lamp_on && logical_map[i][j].num == current) {
                    current_i = i;
                    current_j = j;
                }
                if (logical_map[i][j].type == lamp_off && logical_map[i][j].num == next) {
                    next_i = i;
                    next_j = j;
                }
            }
        }
        update(current_i, current_j, next_i, next_j, "lamp");

    } else if (character == IL) {
        printf("\n which exit do you want to free?\n\n 0)main menu\n 1 2\n");
        scanf("%d", &current);
        while (current==0){
            system("cls");
            menu(during, false);
            printf("\n which exit do you want to free?\n\n 0)main menu\n 1 2\n");
            scanf("%d", &current);
        }
        printf(" which exit do you want to block?\n\n 0)main menu\n 1 2\n");
        scanf("%d", &next);
        while (next==0){
            system("cls");
            menu(during, false);
            printf(" which exit do you want to block?\n\n 0)main menu\n 1 2\n");
            scanf("%d", &next);
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 13; j++) {
                if (logical_map[i][j].type==exit_off && logical_map[i][j].num == current) {
                    current_i = i;
                    current_j = j;
                }
                if (logical_map[i][j].type==exit_on && logical_map[i][j].num == next) {
                    next_i = i;
                    next_j = j;
                }
            }
        }
        update(current_i, current_j, next_i, next_j, "exit");

    } else if (character == SG) {
        int max_moves=3, direction;
        cell copy[9][13];
        while (max_moves>0){
            for (int i=0; i<9; i++){
                for (int j=0; j<13; j++){
                    copy[i][j]=logical_map[i][j];
                }
            }
            choice=0;
            sleep(1);
            system("cls");
            print_map();
            printf("\n choose the character you want to bring closer to yourself:\n 0)main menu\n");
            printf(" 1)SH 2)JW 3)JS 4)IL 5)MS 6)WG 7)JB\n");
            scanf("%d", &current);
            while (current==0){
                system("cls");
                menu(during, false);
                printf("\n choose the character you want to bring closer to yourself:\n 0)main menu\n");
                printf(" 1)SH 2)JW 3)JS 4)IL 5)MS 6)WG 7)JB\n");
                scanf("%d", &current);
            }
            if (current>5)
                current++;
            for (int i=0; i<9; i++){
                for (int j=0; j<13; j++){
                    if (logical_map[i][j].character==current){
                        current_i=i; current_j=j;
                    }
                }
            }
            while (choice!=2 && max_moves>0) {
                printf("\n choose a direction for %s\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n",
                       ToString(current));
                scanf("%d", &direction);
                while (direction == 0) {
                    system("cls");
                    menu(during, false);
                    printf("\n choose a direction for %s\n    0)main menu\n    1)up    3)up_right    5)up_left\n    2)down  4)down_right  6)down_left\n",
                           ToString(current));
                    scanf("%d", &direction);
                }
                find_coordinates(current_i, current_j, &next_i, &next_j, direction);
                if (next_i >= 0 && next_i <= 8 && next_j >= 0 && next_j <= 12 &&
                    (copy[next_i][next_j].type > 4 || copy[next_i][next_j].type == 1) &&
                    !(max_moves==1 && copy[next_i][next_j].character!=0)) {
                    update(current_i, current_j, next_i, next_j, ToString(current));
                    copy[current_i][current_j].type=house;
                    current_i=next_i; current_j=next_j;
                    max_moves--;
                    if (logical_map[current_i][current_j].character== current && max_moves>0) {
                        printf("\n another move for this character?\n 1)yep 2)nope\n");
                        scanf("%d", &choice);
                    }
                } else {
                    printf2(12, "\n %s can't go there\n", ToString(current))
                    sleep(1);
                    system("cls");
                    print_map();
                }
            }
        }

    } else if (character == WG) {
        printf("\n which character do you want to change place with?\n\n");
        printf(" 0)main menu\n 1)SH 2)JW 3)JS 4)IL 5)MS 6)SG 7)JB\n");
        scanf("%d", &next);
        while (next==0){
            system("cls");
            menu(during, false);
            printf("\n which character do you want to change place with?\n\n");
            printf(" 0)main menu\n 1)SH 2)JW 3)JS 4)IL 5)MS 6)SG 7)JB\n");
            scanf("%d", &next);
        }
        if (next == 7)
            next++;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 13; j++) {
                if (logical_map[i][j].character == WG) {
                    current_i = i;
                    current_j = j;
                }
                if (logical_map[i][j].character == next) {
                    next_i = i;
                    next_j = j;
                }
            }
        }
        update(current_i, current_j, next_i, next_j, "WG");
        update(next_i, next_j, current_i, current_j, ToString(next));
        logical_map[next_i][next_j].character = WG;
        logical_map[current_i][current_j].character = next;

    } else if (character == JB) {
        printf("\n which manhole do you want to open?\n\n 0)main menu\n 1 2\n");
        scanf("%d", &current);
        while (current==0){
            system("cls");
            menu(during,false);
            printf("\n which manhole do you want to open?\n\n 0)main menu\n 1 2\n");
            scanf("%d", &current);
        }
        printf(" which manhole do you want to cover?\n\n 0)main menu\n 1 2 3 4 5 6\n");
        scanf("%d", &next);
        while (next==0){
            system("cls");
            menu(during, false);
            printf(" which manhole do you want to cover?\n\n 0)main menu\n 1 2 3 4 5 6\n");
            scanf("%d", &next);
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 13; j++) {
                if (logical_map[i][j].type == close_well && logical_map[i][j].num == current) {
                    current_i = i;
                    current_j = j;
                }
                if (logical_map[i][j].type == open_well && logical_map[i][j].num == next) {
                    next_i = i;
                    next_j = j;
                }
            }
        }
        update(current_i, current_j, next_i, next_j, "manhole");
    }
}

// (رابط بین کاربر و توابع ability و move)
void act(int choice, struct node *actions) {
    int i = 1, direction, option;
    while (i < choice) {
        actions = actions->next;
        i++;
    }
    if (actions->type == SH || actions->type == JW) {
        move(actions->type);
        if (mr_jack.end_of_game == true)
            return;
        ability(actions->type);
    } else if (actions->type == JB || actions->type == JS || actions->type == SG || actions->type == IL) {
        printf("\n do you want to move first or use your ability?\n\n 0)main menu\n 1)move\n 2)ability\n");
        scanf("%d", &option);
        while (option==0){
            system("cls");
            menu(during, false);
            printf("\n do you want to move first or use your ability?\n\n 0)main menu\n 1)move\n 2)ability\n");
            scanf("%d", &option);
        }
        if (option == 1) {
            move(actions->type);
            if (mr_jack.end_of_game == true)
                return;
            ability(actions->type);
        } else {
            ability(actions->type);
            move(actions->type);
            if (mr_jack.end_of_game == true)
                return;
        }
    } else if (actions->type == WG) {
        printf("\n do you want to move or use your ability? you can only do one\n\n 0)main menu\n 1)move\n 2)ability\n");
        scanf("%d", &option);
        while (option==0){
            system("cls");
            menu(during, false);
            printf("\n do you want to move or use your ability? you can only do one\n\n 0)main menu\n 1)move\n 2)ability\n");
            scanf("%d", &option);
        }
        if (option == 1) {
            move(actions->type);
            if (mr_jack.end_of_game == true)
                return;
        } else {
            ability(actions->type);
        }
    } else {
        move(actions->type);
        if (mr_jack.end_of_game == true)
            return;
    }
}

// این تابع توکن لامپ های 1 تا 4 را در اخر هر راند از نقشه خارج می کند.
void remove_lamp(int round) {
    bool flag = false;
    char symbol[10] = {'O', 'F', 'F', ' '};
    char number[2];
    number[1] = '\0';
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            if (logical_map[i][j].type == lamp_on && logical_map[i][j].num == round) {
                logical_map[i][j].type = lamp_off;
                logical_map[i][j].num += 2;
                number[0] = logical_map[i][j].num + 48;
                strcat(symbol, number);
                if (j % 2 == 0) {
                    strncpy(&graphic_map[4 * i + 4][8 * j + 3], symbol, strlen(symbol));
                    flag = true;
                    break;
                } else {
                    strncpy(&graphic_map[4 * i + 2][(j / 2) * 16 + 11], symbol, strlen(symbol));
                    flag = true;
                    break;
                }
            }
        }
        if (flag == true)
            break;
    }
    save_graphic_map();
    sleep(1);
    system("cls");
    print_map();
}

void call_for_witness() {
    int wi, wj, next_i, next_j, i;
    for (i = 0; i < 8; i++) {
        characters_status[i].visible = false;
    }
    for (i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            if (logical_map[i][j].character != 0) {
                for (int k=1; k<=6; k++){
                    find_coordinates(i, j, &next_i, &next_j, k);
                    if (next_i>0 && next_i<9 && next_j>0 && next_j<13 &&
                        (logical_map[next_i][next_j].character!=0 || logical_map[next_i][next_j].type==lamp_on)){
                        characters_status[logical_map[i][j].character - 1].visible=true;
                    }
                }
                if (logical_map[i][j].character == JW) {
                    wi = i;
                    wj = j;
                }
            }
        }
    }
    find_coordinates(wi, wj, &next_i, &next_j, lantern_direction);
    while (lantern_direction != 0 && logical_map[next_i][next_j].type != house &&
           logical_map[next_i][next_j].type != lamp_off &&
           logical_map[next_i][next_j].type != extra) {
        if (logical_map[next_i][next_j].character != 0) {
            characters_status[logical_map[next_i][next_j].character - 1].visible = true;
        }
        find_coordinates(next_i, next_j, &next_i, &next_j, lantern_direction);
    }
    for (i = 0; i < 8; i++) {
        if (characters_status[i].type == mr_jack.character) {
            if (characters_status[i].visible == true) {
                printf2(12, "%s", "\n Mr.Jack is visible\n")
                mr_jack.visible = true;
                break;
            } else {
                printf2(12, "%s", "\n Mr.Jack is invisible\n")
                mr_jack.visible = false;
                break;
            }
        }
    }
    char *ptr;
    for (i = 0; i < 8; i++) {
        if (characters_status[i].visible != mr_jack.visible) {
            characters_status[i].innocent = true;
            for (int row = 0; row < 39; row++) {
                ptr = strstr(graphic_map[row], ToString(characters_status[i].type));
                if (ptr != NULL) {
                    strncpy(ptr - 3, "(i)", 3);
                    break;
                }
            }
        }
    }
    save_graphic_map();
    sleep(1);
    system("cls");
    print_map();
}

