#ifndef ALLEGRO_H
#define ALLEGRO_H

#include "struct.h"
#include "game.h"

void render_score(AllegroStructure *allegro, GAMEstructure *game);

void render_menu(AllegroStructure *allegro, GAMEstructure *game);

void render_switch(GAMEstructure *game, AllegroStructure *allegro, int c);

void draw_switch(int element, int y, int x, AllegroStructure *allegro, GAMEstructure *game, int c);

void render(GAMEstructure *game, AllegroStructure *allegro);

void draw(int element, int y, int x, AllegroStructure *allegro);

void init_allegro(AllegroStructure *allegro);

#endif