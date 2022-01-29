#include "map.h"

void make_empty() {
    for (int i = 0; i < 39; i++) {
        for (int j = 0; j < 107; j++) {
            graphic_map[i][j] = ' ';
        }
    }
}

void creat_backslash(int i, int j) {
    graphic_map[i][j] = graphic_map[i + 1][j + 1] = graphic_map[i + 2][j + 2] = '*';
}

void creat_slash(int i, int j) {
    graphic_map[i][j] = graphic_map[i - 1][j + 1] = graphic_map[i - 2][j + 2] = '*';
}

void creat_bottom(int i, int j) {
    if (j == 106)
        return;
    graphic_map[i][j] = graphic_map[i][j + 3] = graphic_map[i][j + 6] = '*';
}

// این تابع واحد سازنده نقشه را ایجاد کرده در ارایه ذخیره میکند
void creat_cell(int i, int j) {
    creat_slash(i, j);
    creat_bottom(i - 2, j + 2);
    creat_backslash(i, j);
    creat_bottom(i + 2, j + 2);
    creat_slash(i + 2, j + 8);
    creat_backslash(i - 2, j + 8);
    creat_bottom(i, j + 10);
}

// این تابع با کنار هم گذاشتن واحد های سازنده، کل نقشه را ایجاد میکند
void creat_map() {
    make_empty();
    int i = 2, j;
    for (j = 8; j <= 88; j += 16) {
        creat_cell(i, j);
    }
    for (i = 4; i < 39; i += 4) {
        for (j = 0; j <= 96; j += 16) {
            creat_cell(i, j);
        }
    }
}

// این تابع مپ منطقی اولیه بازی که شامل اطلاعات هر خانه است را تشکیل می دهد
void fill_logical_map() {
    FILE *fp = fopen("map_info.txt", "r");
    int al = 1, il = 1, m = 1, mc = 1;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            fscanf(fp, "%d", &logical_map[i][j].type);
            if (logical_map[i][j].type == lamp_on) {
                logical_map[i][j].num = al;
                al++;
            } else if (logical_map[i][j].type == lamp_off) {
                logical_map[i][j].num = il;
                il++;
            } else if (logical_map[i][j].type == open_well) {
                logical_map[i][j].num = m;
                m++;
            } else if (logical_map[i][j].type == close_well) {
                logical_map[i][j].num = mc;
                mc++;
            }
        }
    }
    fclose(fp);
    logical_map[0][0].num = 1;
    logical_map[7][11].num = 2;
    logical_map[8][0].num = 2;
    logical_map[0][12].num = 1;
    logical_map[0][4].character = WG;
    logical_map[2][6].character = JS;
    logical_map[3][8].character = JB;
    logical_map[5][6].character = SH;
    logical_map[4][4].character = IL;
    logical_map[4][0].character = JW;
    logical_map[3][12].character = SG;
    logical_map[7][8].character = MS;
}

// این تابع با استفاده از مپ منطقی، اطلاهات را در مپ گرافیکی وارد می کند.
void fill_graphic_map() {
    char symbol[10];
    char num[2];
    num[1] = '\0';
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 13; j++) {
            if (logical_map[i][j].character!=0){
                if (j % 2 == 0) {
                    strncpy(&graphic_map[4 * i + 3][8 * j + 6], ToString(logical_map[i][j].character), 2);
                } else {
                    strncpy(&graphic_map[4 * i + 1][(j / 2) * 16 + 14], ToString(logical_map[i][j].character), 2);
                }
            }
            if (logical_map[i][j].type == house) {
                strcpy(symbol, "#");
            } else if (logical_map[i][j].type == extra) {
                strcpy(symbol, "###");
            } else if (logical_map[i][j].type == lamp_on) {
                strcpy(symbol, "ON ");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
            } else if (logical_map[i][j].type == lamp_off) {
                strcpy(symbol, "OFF ");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
            } else if (logical_map[i][j].type == close_well) {
                strcpy(symbol, "(x");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
                strcat(symbol, ")");
            } else if (logical_map[i][j].type == open_well) {
                strcpy(symbol, "(");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
                strcat(symbol, ")");
            } else if (logical_map[i][j].type == exit_on){
                strcpy(symbol, "E");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
            } else if (logical_map[i][j].type == exit_off){
                strcpy(symbol, "B");
                num[0] = logical_map[i][j].num + 48;
                strcat(symbol, num);
            } else {
                continue;
            }
            if (j % 2 == 0) {
                strncpy(&graphic_map[4 * i + 4][8 * j + 3], symbol, strlen(symbol));
            } else {
                strncpy(&graphic_map[4 * i + 2][(j / 2) * 16 + 11], symbol, strlen(symbol));
            }
        }
    }
}

//این تابع کاربر را راهنمایی می کند تا مپ شخصی خود را ایجاد کند.
void creat_personal_map(){
    int i,j, choice=0, character, row, column;
    COORD C;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    system("cls");
    printf(" ok, let's creat your own personal map. \n the map has 9 rows and 13 columns and there are different cells in the map which you can use.\n each cell has an especial number. place these numbers in the table below to define cell types.\n");
    printf2(9,"%s", " 0: extra cells around the map     3: lit-lamps            5: open-manholes            7: open exits   \n 1: empty cells                    4: shut-off lamps       6: covered-manholes         8: closed exits\n 2: houses\n")
    printf2(12, "%s", "\n   0  1  2  3  4  5  6  7  8  9 10 11 12\n")
    for (i=0; i<9; i++){
        printf2(12, " %d ", i)
        for (j=0; j<13; j++){
            scanf("%d", &logical_map[i][j].type);
        }
    }
    printf("\n do you want to finalize your map?\n 1)finalize 2)edit\n");
    scanf("%d", &choice);
    while (choice == 2){
        printf(" which cell do you want to edit?\n row: ");
        scanf("%d", &i );
        printf(" column: ");
        scanf("%d", &j);
        C.X=(j+1) * 3; C.Y=i+8;
        printf("\n enter new value \n");
        GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
        SetConsoleCursorPosition(hConsole,C);
        scanf("%d", &logical_map[i][j].type);
        C.X= csbiInfo.dwCursorPosition.X;
        C.Y= csbiInfo.dwCursorPosition.Y;
        SetConsoleCursorPosition(hConsole,C);
        printf("\n do you want to finalize your map?\n 1)finalize 2)edit\n");
        scanf("%d", &choice);
    }
    printf("\n alright, now please enter the position of different characters. \n note that you can only place the characters in empty cells(manholes and exits are also considered empty).\n the table below shows available cells to put characters in. 1 means empty and 0 means full.\n");
    printf2(12, "%s", "   0  1  2  3  4  5  6  7  8  9 10 11 12\n")
    for (i=0; i<9 ;i++){
        printf2(12, " %d ", i);
        for (j=0; j<13; j++){
            if (logical_map[i][j].type> lamp_off || logical_map[i][j].type==empty){
                printf("1  ");
            } else{
                printf("0  ");
            }
        }
        printf("\n");
    }
    for (character=SH; character<=JB; character++){
        printf("\n %s: row: ", ToString(character));
        scanf("%d", &row);
        printf("     column: ");
        scanf("%d", &column);
        while (logical_map[row][column].type==extra || (logical_map[row][column].type>empty && logical_map[row][column].type<open_well)){
            printf2(12, "\n %s\n", "you can't place the character in that cell!");
            printf(" enter again! row: ");
            scanf("%d", &row);
            printf("              column: ");
            scanf("%d", &column);
        }
        while (logical_map[row][column].character!=0){
            printf2(12, "\n %s\n", "you've already placed a character in that cell")
            printf(" enter again! row: ");
            scanf("%d", &row);
            printf("              column: ");
            scanf("%d", &column);
        }
        logical_map[row][column].character=character;
        printf2(12, "%s", "\n   0  1  2  3  4  5  6  7  8  9 10 11 12\n")
        for (i=0; i<9 ;i++){
            printf2(12, " %d ", i);
            for (j=0; j<13; j++){
                if(logical_map[i][j].character!=0){
                    printf2(12, "%s ", ToString(logical_map[i][j].character))
                    continue;
                }
                if (logical_map[i][j].type> lamp_off || logical_map[i][j].type==empty){
                    printf("1  ");
                } else{
                    printf("0  ");
                }
            }
            printf("\n");
        }
    }
    printf2(10, "\n %s\n", "your map is ready, now you can start playing on it.")
    sleep(5);
    system("cls");
    int al = 1, il = 1, e = 1, b = 1, m = 1, mc = 1;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 13; j++) {
            if (logical_map[i][j].type == lamp_on) {
                logical_map[i][j].num = al;
                al++;
            } else if (logical_map[i][j].type == lamp_off) {
                logical_map[i][j].num = il;
                il++;
            } else if (logical_map[i][j].type == open_well) {
                logical_map[i][j].num = m;
                m++;
            } else if (logical_map[i][j].type == close_well) {
                logical_map[i][j].num = mc;
                mc++;
            }
            else if (logical_map[i][j].type == exit_on){
                logical_map[i][j].num = e;
                e++;
            } else if (logical_map[i][j].type == exit_off){
                logical_map[i][j].num = b;
                b++;
            }
        }
    }
}

//این تابع ابتدا رنگ هر یک از خانه های مپ گرافیکی را تعیین کرده و سپس ان را پرینت میکند
void print_map() {
    int i, j, current_j, num;
    for (i=0; i<39; i++){
        for (j=0; j<108; j++){
            color[i][j] = 7;
        }
    }
    for (i = 0; i < 39; i++) {
        for (j = 0; j < 108; j++) {
            if (graphic_map[i][j] == 'O') {
                if (graphic_map[i][j + 1] == 'N') {
                    num=14;
                } else {
                    num=11;
                }
            }
            else if (graphic_map[i][j] == '('){
                if (graphic_map[i][j + 1] == 'x') {
                    num=13;
                } else if (graphic_map[i][j + 1] != 'i') {
                    num=9;
                } else
                    continue;
            }
            else if (graphic_map[i][j] == '#' && graphic_map[i][j - 1] == ' '){
                num=2;
            }
            else if ((graphic_map[i][j] == 'B' || graphic_map[i][j] == 'E') && (graphic_map[i][j + 1]=='1' ||  graphic_map[i][j + 1]=='2')){
                num=12;
            }
            else if ((graphic_map[i][j] == 'S' || graphic_map[i][j] == 'J' || graphic_map[i][j] == 'W' ||
                      graphic_map[i][j] == 'M' || graphic_map[i][j] == 'I') && graphic_map[i][j + 1] != ' ')
            {
                if (graphic_map[i][j - 1] == ')'){
                    color[i][j - 3] = color[i][j - 2] = color[i][j - 1] = color[i][j] = color[i][j + 1] =10;
                }
                else{
                    color[i][j] = color[i][j + 1] = 4;
                }
                continue;
            }
            else{
                continue;
            }
            for (current_j = j; current_j < j + 7; current_j++) {
                color[i][current_j] = num;
            }
        }
    }
    printf2(15, "%s\n%s\n\n", "================================================================"," map guid:")
    printf2(13, "%s", " (x) : covered manholes     ")
    printf2(9, "%s\n", "( ): open manholes")
    printf2(14, "%s", "  ON : lit lamps            ")
    printf2(11, "%s\n", "OFF: shut-off lamps")
    printf2(2, "%s", "  #  : houses               ###: extra cells around the map\n")
    printf2(12, "%s\n", "  B  : closed exits          E : open exits")
    printf2(15, "%s\n\n", "================================================================")
    for (i=0; i<39; i++) {
        for (j = 0; j < 108; j++) {
            printf2(color[i][j], "%c", graphic_map[i][j]);
        }
        printf("\n");
    }
}
