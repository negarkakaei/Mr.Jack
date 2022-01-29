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
    int temp;
    int al = 1, il = 1, e = 1, b = 1, m = 1, mc = 1;
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
    strncpy(&graphic_map[4][99], "B1", 2);
    strncpy(&graphic_map[36][3], "B2", 2);
    strncpy(&graphic_map[4][3], "E1", 2);
    strncpy(&graphic_map[30][91], "E2", 2);
    strncpy(&graphic_map[3][38], "WG", 2);
    strncpy(&graphic_map[11][54], "JS", 2);
    strncpy(&graphic_map[15][70], "JB", 2);
    strncpy(&graphic_map[23][54], "SH", 2);
    strncpy(&graphic_map[19][38], "IL", 2);
    strncpy(&graphic_map[19][6], "JW", 2);
    strncpy(&graphic_map[15][102], "SG", 2);
    strncpy(&graphic_map[31][70], "MS", 2);
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
    for (i=0; i<39; i++){
        for (j=0; j<108; j++){
            printf2(color[i][j], "%c", graphic_map[i][j]);
        }
        printf("\n");
    }

}
