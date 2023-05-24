#ifndef GAME_H
#define GAME_H

#include "struct.h"
#include "allegro.h"

void explosion(GAMEstructure *game);

void add_jewels(GAMEstructure *game);

void gravity(GAMEstructure *game);

void check_map(GAMEstructure *game, AllegroStructure *allegro);

void clear_switch(GAMEstructure *game);

void switch_jewel(AllegroStructure *allegro, GAMEstructure *game);

void click(AllegroStructure *allegro, GAMEstructure *game);

void init_map(int **map);

int **alloc_map();

void reset_game(GAMEstructure *game);

#endif