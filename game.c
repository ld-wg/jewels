#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include "struct.h"
#include "allegro.h"
#include "game.h"

#define CLEAR 99999
#define EMPTY 88888
#define MAPSIZE 8

void clear_color(GAMEstructure *game, int element, int beam, AllegroStructure *allegro) {
    if(allegro->flash) game->score += 5000;
    else game->score += 50;
    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            if(game->map[j][i] == element % 10) {
                game->map[j][i] = beam;
                game->time += 0.1;
                game->score += 5;
            }
        }
    }
}

void explosion(GAMEstructure *game) {
    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            if ((game->map[j][i] == BEAMXLIMIT) || (game->map[j][i] == BEAMYLIMIT)) {
                game->map[j][i] = EMPTY;
            }
            else if((game->map[j][i] >= BEAMX && game->map[j][i] < BEAMXLIMIT) || 
                (game->map[j][i] >= BEAMY && game->map[j][i] < BEAMYLIMIT)) {
                game->map[j][i]++;
            }
        }
    }
}

void add_jewels(GAMEstructure *game) {
    for(int i = 0; i < MAPSIZE; i++) {
        if(game->map[i][0] == EMPTY) game->map[i][0] = rand() % 5;
    }
}

void gravity(GAMEstructure *game) {
    // -2 no contador do eixo y pois a ultima linha nunca cai, entao nao e necessario verificar
    bool fall = false;
    for(int i = MAPSIZE - 2; i >= 0; i--) {
        for(int j = MAPSIZE - 1; j >= 0; j--) {
            if(game->map[j][i+1] == EMPTY) {
                game->map[j][i+1] = game->map[j][i];
                game->map[j][i] = EMPTY;
                fall = true;
            }
        }
    }
    game->is_falling = fall;
}

int is_equal_y(int **map, int i, int j) {
    int n = 0;
    if((i+1) < MAPSIZE && map[j][i] < BEAMY) {
        if(map[j][i] % 10 == map[j][i+1] % 10) {
            n += is_equal_y(map, i+1, j);
            n++;
            return n;
        }
    }
    return n;
}

int is_equal_x(int **map, int i, int j) {
    int n = 0;
    if((j+1) < MAPSIZE && map[j][i] < BEAMY) {
        if(map[j][i] % 10 == map[j+1][i] % 10) {
            n += is_equal_x(map, i, j+1);
            n++;
            return n;
        }
    }
    return n;
}

bool check_if_valid(GAMEstructure *game) {
    int aux_y[MAPSIZE][MAPSIZE];
    int aux_x[MAPSIZE][MAPSIZE];
    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            aux_x[j][i] = is_equal_x(game->map, i, j);
            aux_y[j][i] = is_equal_y(game->map, i, j);
        }
    }

    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            if(aux_x[j][i] > 1 || aux_y[j][i] > 1) {
                return true;
            }
        }
    }
    return false;
}

void check_map(GAMEstructure *game, AllegroStructure *allegro) {
    int aux_y[MAPSIZE][MAPSIZE];
    int aux_x[MAPSIZE][MAPSIZE];
    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            aux_x[j][i] = is_equal_x(game->map, i, j);
            aux_y[j][i] = is_equal_y(game->map, i, j);
        }
    }

    for(int i = 0; i < MAPSIZE; i++) {
        for(int j = 0; j < MAPSIZE; j++) {
            if((aux_x[j][i] > 2) && (game->map[j][i] < BEAMY)) {
                if(game->map[j][i] < 10) game->map[j][i] = game->map[j][i] + 10;
            }
            else if((aux_y[j][i] > 2) && (game->map[j][i] < BEAMY)) {
                if(game->map[j][i] < 10) game->map[j][i] = game->map[j][i] + 10;
            }
            else {
                if(aux_x[j][i] > 1) {
                    for(int k = 0; k <= aux_x[j][i]; k++) {
                        if(game->map[j+k][i] > 9 && game->map[j+k][i] < BEAMY) {
                            // if((i+1) >= 0 && (i+1) < MAPSIZE) {
                            //     game->map[j+k][i+1] = BEAMX;
                            //     if((j+k+1) >= 0 && (j+k+1) < MAPSIZE) game->map[j+k+1][i+1] = BEAMX;
                            //     if((j+k-1) >= 0 && (j+k-1) < MAPSIZE) game->map[j+k-1][i+1] = BEAMX;
                            // } 
                            // if((i-1) >= 0 && (i-1) < MAPSIZE) {
                            //     game->map[j+k][i-1] = BEAMX;
                            //     if((j+k+1) >= 0 && (j+k+1) < MAPSIZE) game->map[j+k+1][i-1] = BEAMX;
                            //     if((j+k-1) >= 0 && (j+k-1) < MAPSIZE) game->map[j+k-1][i-1] = BEAMX;
                            // }
                            clear_color(game, game->map[j+k][i], BEAMX, allegro);
                            al_play_sample(allegro->sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
                        }
                        game->map[j+k][i] = BEAMX;
                        if(allegro->flash) {
                            game->time += 1;
                            game->score += 50;
                        } else {
                            game->time += 0.2;
                            game->score += 5;
                        }
                        al_play_sample(allegro->sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);  
                    }
                }
                if(aux_y[j][i] > 1) {
                    for(int k = 0; k <= aux_y[j][i]; k++) {
                        if(game->map[j][i+k] > 9 && game->map[j][i+k] < BEAMY) {
                            // if((j+1) >= 0 && (j+1) < MAPSIZE) {
                            //     game->map[j+1][i+k] = BEAMY;
                            //     if((i+k+1) >= 0 && (i+k+1) < MAPSIZE) game->map[j+1][i+k+1] = BEAMY;
                            //     if((i+k-1) >= 0 && (i+k-1) < MAPSIZE) game->map[j-1][i+k+1] = BEAMY;

                            // }
                            // if((j-1) >= 0 && (j-1) < MAPSIZE) game->map[j-1][i+k] = BEAMY;
                            clear_color(game, game->map[j][i+k], BEAMX, allegro);
                            al_play_sample(allegro->sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
                        }
                        game->map[j][i+k] = BEAMY;
                        if(allegro->flash) {
                            game->time += 1;
                            game->score += 50;
                        } else {
                            game->time += 0.2;
                            game->score += 5;
                        }
                        al_play_sample(allegro->sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
                    }
                }
            }
        }
    }
}

void clear_switch(GAMEstructure *game) {
    game->switch_1->x = CLEAR;
    game->switch_1->y = CLEAR;
    game->switch_2->x = CLEAR;
    game->switch_2->y = CLEAR;
}

void switch_jewel(AllegroStructure *allegro, GAMEstructure *game) {
    bool redraw = true;
    for(int i = 1; i <= 64; i++) {
        al_wait_for_event(allegro->queue, &allegro->event);

        if (redraw && al_is_event_queue_empty(allegro->queue)) {
            //print_ascii_map(game->map);
            render_switch(game, allegro, i);
            redraw = false;
        }

        if(allegro->event.type == ALLEGRO_EVENT_TIMER) {
            if (allegro->event.timer.source == allegro->tick) {
                redraw = true;   
            }
        }
    }

    int temp = game->map[game->switch_1->x][game->switch_1->y];
    game->map[game->switch_1->x][game->switch_1->y] = game->map[game->switch_2->x][game->switch_2->y];
    game->map[game->switch_2->x][game->switch_2->y] = temp;
}

void click(AllegroStructure *allegro, GAMEstructure *game) {
    if(allegro->pos_x >= 0 && allegro->pos_x < MAPSIZE && allegro->pos_y >= 0 && allegro->pos_y < MAPSIZE) {        
        if(game->switch_1->x == CLEAR) {
            game->switch_1->x = allegro->pos_x;
            game->switch_1->y = allegro->pos_y;
        } else {
            if (allegro->pos_x == game->switch_1->x && allegro->pos_y == game->switch_1->y) clear_switch(game);
            if(abs(allegro->pos_x - game->switch_1->x) <= 1 && abs(allegro->pos_y - game->switch_1->y) <= 1 && ((allegro->pos_x == game->switch_1->x && allegro->pos_y != game->switch_1->y) || (allegro->pos_x != game->switch_1->x && allegro->pos_y == game->switch_1->y))) {
                game->switch_2->x = allegro->pos_x;
                game->switch_2->y = allegro->pos_y;
                switch_jewel(allegro, game);
                if(check_if_valid(game)) clear_switch(game);
                else {
                    switch_jewel(allegro, game);
                    clear_switch(game);
                } 
            }
        }
    }
}

void init_map(int **map) {
    srand(time(NULL)); // randomize seed

    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            map[j][i] = rand() % 5;
        }
    }
}

int **alloc_map() {
    int **map;
    map = malloc(sizeof(int *) * MAPSIZE);
    for (int i = 0; i < MAPSIZE; i++)
        map[i] = malloc(sizeof(int) * MAPSIZE);
    return map;
}


void reset_game(GAMEstructure *game) {
    if(game->switch_1 != NULL) free(game->switch_1);
    if(game->switch_2 != NULL) free(game->switch_2);
    game->map = alloc_map();
    game->time = 20;
    game->score = 0;
    game->is_falling = false;
    game->switch_1 = malloc(sizeof(coord));
    game->switch_2 = malloc(sizeof(coord));
    game->switch_1->x = CLEAR;
    game->switch_1->y = CLEAR;
    game->switch_2->x = CLEAR;
    game->switch_2->y = CLEAR;
}