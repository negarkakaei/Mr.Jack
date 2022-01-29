#ifndef MAIN_C_ACTION_H
#define MAIN_C_ACTION_H
#include "basic.h"

void find_coordinates(int current_i, int current_j, int *next_i, int *next_j, enum direction choice);
void update(int current_i, int current_j, int next_i, int next_j, char object[10]);
bool valid(enum character_type character, bool last_move, int next_i, int next_j);
void move(enum character_type character);
void sherlock();
void ability(enum character_type character);
void act(int choice, struct node *actions);
void remove_lamp(int round);
void call_for_witness();

#endif //MAIN_C_ACTION_H
