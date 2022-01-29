#ifndef MAIN_C_BASIC_H
#define MAIN_C_BASIC_H

#ifndef hConsole
#define hConsole GetStdHandle(STD_OUTPUT_HANDLE)

#ifndef printf2
#define printf2(color, format, ...) \
SetConsoleTextAttribute(hConsole, color); \
printf(format, __VA_ARGS__); \
SetConsoleTextAttribute(hConsole, 07);

#include "list.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>
#include "map.h"
#include <windows.h>
#include <winuser.h>

extern int round_cnt, turn;
extern int player, lantern_direction;
extern char **alibi, detective_cards[7][3], jack_cards[7][3];
extern int a, d, c;
extern bool load, prev_game, two_characters,arrest;
extern int color[39][108];

enum cell_type {
    extra, empty, house, lamp_on, lamp_off, open_well, close_well, exit_on, exit_off
};
enum character_type {
    SH = 1, JW, JS, IL, MS, SG, WG, JB,
};
enum condition {
    Jack = 1, detective = 2, start=1, during=2
};
enum direction {
    up = 1, down, up_right, down_right, up_left, down_left
};

// استراکت هر سلول شامل اطلاعات نوع سلول، وضعیت چاه و چراغ و خروجی در صورت وجود، و کارکتر موجود درآن خانه
typedef struct {
    enum cell_type type;
    enum character_type character;
    int num;
} cell;
cell logical_map[9][13];

// استراکت هر شخصیت شامل اطلاعات نام کارکتر، مرئی بودن و بی گناه بودن
struct node {
    enum character_type type;
    bool visible;
    bool innocent;
    struct node *next;
};
struct node characters_status[8];

// استارکت مستر جک برای تعیین وضعیت برد و باخت بازی و مرئی بودن جک
typedef struct {
    enum character_type character;
    bool end_of_game;
    bool visible;
} jack;
jack mr_jack;

typedef struct {
    int row;
    int column;
} coordinate;
coordinate prev_cells[5];
int p;

//لینک لیست list شامل هر 8 کارت شخصیت است که در ابتدای راند های فرد shuffle می شود.
// لینک لیست this_round شامل 4 کارت اول لیست اصلیست و در راند های فرد استفاده می شود.
//لینک لیست next_round شامل 4 کارت دوم است و در راند های زوج استفاده می شود
struct node *list, *this_round, *next_round;

// نقشه بازی متشکل از شش ضلعی هایی به طول ضلع 3
char graphic_map[39][108];

char *ToString(enum character_type character);
enum character_type ToEnum(char str[3]);
void save_graphic_map();
void save_logical_map();
void load_graphic();
void load_logic();
void menu(enum condition input, bool exit_option);
void print_actions(struct node *actions);
#endif
#endif
#endif