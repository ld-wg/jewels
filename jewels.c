#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "game.h"
#include "allegro.h"

#define MAPSIZE 8

enum {INICIO, SCORE, JOGANDO, FIMPART, FIMJOGO} state;

void free_structures(AllegroStructure *allegro, GAMEstructure *game) {
    free(allegro);
    free(game);
}

void open_file(GAMEstructure *game) { 
   game->arq = fopen ("score.txt", "a+") ;
 
   if ( !game->arq ) {
      perror ("Erro ao abrir/criar arquivo de score") ;
      exit (1) ; // encerra o programa com status 1
   }
}

void print_ascii_map(int **map) {
    int i, j;
    
    for (i = 0; i < MAPSIZE; i++) {
        for (j = 0; j < MAPSIZE; j++) {
            printf("%i ", map[j][i]);
        }
        printf ("\n");
    }
    printf("\n");
}

void state_init(AllegroStructure *allegro, GAMEstructure *game) {
    reset_game(game);
    init_map(game->map);

    al_wait_for_event(allegro->queue, &allegro->event);
    allegro->redraw = true;

    if (allegro->redraw && al_is_event_queue_empty(allegro->queue)) {
        render_menu(allegro, game);
        allegro->redraw = false;
        al_flip_display();
    }

    switch (allegro->event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (allegro->event.timer.source == allegro->tick) allegro->redraw = true;
        break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            allegro->pos_x = (int) floor(allegro->event.mouse.x/64);
            allegro->pos_y = (int) floor(allegro->event.mouse.y/64);
        break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if(allegro->event.mouse.button & 1) {
                if(allegro->pos_x >= 0 && allegro->pos_x < 3 && allegro->pos_y >= 0 && allegro->pos_y < 2)
                    state = JOGANDO;
                if(allegro->pos_x >= 0 && allegro->pos_x < 3 && allegro->pos_y >= 3 && allegro->pos_y < 5)
                    state = SCORE;
                if (allegro->pos_x >= 6 && allegro->pos_x < 8 && allegro->pos_y >= 8 && allegro->pos_y < 10)
                    state = FIMJOGO;
            } 
        break;
    }
}

void state_score(AllegroStructure *allegro, GAMEstructure *game) {
    freopen("score.txt", "r", game->arq);

    al_wait_for_event(allegro->queue, &allegro->event);
    allegro->redraw = true;

    if (allegro->redraw && al_is_event_queue_empty(allegro->queue)) {
        render_score(allegro, game);
        allegro->redraw = false;
        al_flip_display();
    }

    switch (allegro->event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (allegro->event.timer.source == allegro->tick) allegro->redraw = true;
        break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            allegro->pos_x = (int) floor(allegro->event.mouse.x/64);
            allegro->pos_y = (int) floor(allegro->event.mouse.y/64);
        break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if(allegro->event.mouse.button & 1) {
                if (allegro->pos_x >= 6 && allegro->pos_x < 8 && allegro->pos_y >= 8 && allegro->pos_y < 10) {
                    freopen("score.txt", "a+", game->arq);
                    state = INICIO;
                }
            } 
        break;
    }
}

void state_close(AllegroStructure *allegro, GAMEstructure *game) {
    al_destroy_display(allegro->disp);
    free_structures(allegro, game);

}

void state_play(AllegroStructure *allegro, GAMEstructure *game) {
    allegro->redraw = true;

    al_wait_for_event(allegro->queue, &allegro->event);

    if (allegro->redraw && al_is_event_queue_empty(allegro->queue)) {
        render(game, allegro);
        allegro->redraw = false;
    }

    switch (allegro->event.type) {
        case ALLEGRO_EVENT_TIMER:
            if (allegro->event.timer.source == allegro->tick) {
                if(game->is_falling == false) check_map(game, allegro);
                explosion(game);
                add_jewels(game);
                // print_ascii_map(game->map);
            }
            if(allegro->event.timer.source == allegro->timer) {
                if(game->time > 0) {
                    game->time -= 0.1;
                } else {
                    fprintf(game->arq, "%i\n", game->score);
                    state = INICIO;
                }
                
            }
            if (allegro->event.timer.source == allegro->gravity_tick) {
                gravity(game);
            }
                allegro->redraw = true;
        break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            allegro->pos_x = (int) floor(allegro->event.mouse.x/64);
            allegro->pos_y = (int) floor(allegro->event.mouse.y/64);
        break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if(allegro->event.mouse.button & 1) {
                click(allegro, game);
                if (allegro->pos_x >= 6 && allegro->pos_x < 8 && allegro->pos_y >= 8 && allegro->pos_y < 10) {
                    fprintf(game->arq, "%i\n", game->score);
                    state = INICIO;
                }
                else if ((allegro->pos_x >= 0 && allegro->pos_x < 1 && allegro->pos_y >= 9 && allegro->pos_y < 10)) {
                    allegro->flash = !allegro->flash;
                }
            }
        break;
    }
}

int main()
{   
    state = INICIO;

    int end = 1;

    AllegroStructure *allegro = malloc(sizeof(AllegroStructure));
    GAMEstructure *game = malloc(sizeof(GAMEstructure));

    init_allegro(allegro);
    open_file(game);

    while (end)
    switch (state) {
      case INICIO  : state_init(allegro, game);  break;
      case SCORE: state_score(allegro, game); break;
      case JOGANDO : state_play(allegro, game);  break;
      //case FIMPART : state_over();  break;
      case FIMJOGO : 
            fclose (game->arq);
            state_close(allegro, game);
            end = 0;
        break;
      default: break ;
    }
}