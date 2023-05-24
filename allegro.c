#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "allegro.h"

void render_score(AllegroStructure *allegro, GAMEstructure *game) {
    char buffer[LINESIZE+1];
    al_clear_to_color(al_map_rgb(0, 0, 0));
    int c = 0;
    int d = 0;
    while (!feof(game->arq)) {
        if(c > 400) {
            c = 0;
            d = 300;
        }
        char aux[LINESIZE+1]; 
        fscanf(game->arq, "%s", aux);
        if(strcmp(aux, buffer)) al_draw_textf(allegro->font, al_map_rgb(255, 255, 255), 50 + d, 50 + c, 0, "%s", aux);
        strcpy(buffer, aux);
        c += 50;
    }

    al_draw_scaled_bitmap(allegro->sprite_exit, 0, 0, 32, 32, 400, 536, 64, 64, 0);
}

void render_menu(AllegroStructure *allegro, GAMEstructure *game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(allegro->font, al_map_rgb(255, 255, 255), 50, 50, 0, "JOGAR");
    al_draw_text(allegro->font, al_map_rgb(255, 255, 255), 50, 200, 0, "SCORES");
    al_draw_scaled_bitmap(allegro->sprite_exit, 0, 0, 32, 32, 400, 536, 64, 64, 0);
}

void draw_switch(int element, int y, int x, AllegroStructure *allegro, GAMEstructure *game, int c) {
    // al_draw_scaled_bitmap(sprites, cor_x_sprite, cor_y_sprite, tamx, tamy, cor_x_allgr, cor_y_allgr, tamx, tamy, 0)
    ALLEGRO_BITMAP *sprite_x;
    if(element >= BEAMX && element < BEAMXLIMIT) {
        al_draw_scaled_bitmap(allegro->sprite_beam_x, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
    } else if(element >= BEAMY && element < BEAMYLIMIT) {
        al_draw_scaled_bitmap(allegro->sprite_beam_y, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
    } else {
        switch (element) {
            case 0:
                sprite_x = allegro->sprite_1;
                break;
            case 1:
                sprite_x = allegro->sprite_2;
                break;
            case 2:
                sprite_x = allegro->sprite_3;
                break;
            case 3:
                sprite_x = allegro->sprite_4;
                break;
            case 4:
                sprite_x = allegro->sprite_5;
                break;
            case 5:
                sprite_x = allegro->sprite_6;
                break;
            case 10:
                sprite_x = allegro->sprite_12;
                break;
            case 11:
                sprite_x = allegro->sprite_22;
                break;
            case 12:
                sprite_x = allegro->sprite_32;
                break;
            case 13:
                sprite_x = allegro->sprite_42;
                break;
            case 14:
                sprite_x = allegro->sprite_52;
                break;
            case 15:
                sprite_x = allegro->sprite_62;
                break;
        }
        if(game->switch_1->x == x && game->switch_1->y == y) {
            // se elemento estiver a direita do switch_2
            if(game->switch_1->x > game->switch_2->x) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, (x * 64) - c, y * 64, 64, 64, 0);
            // se elemento estiver a esquerda do switch_2
            if(game->switch_1->x < game->switch_2->x) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, (x * 64) + c, y * 64, 64, 64, 0);
            // se elemento estiver a cima do switch_2
            if(game->switch_1->y > game->switch_2->y) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, x * 64, (y * 64) - c, 64, 64, 0);
            // se elemento estiver em baixo do switch_2
            if(game->switch_1->y < game->switch_2->y) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, x * 64, (y * 64) + c, 64, 64, 0);
        } 
        // se elemento for o switch_2
        else if(game->switch_2->x == x && game->switch_2->y == y) {
            // se elemento estiver a direita do switch_1
            if(game->switch_2->x > game->switch_1->x) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, (x * 64) - c, y * 64, 64, 64, 0);
            // se elemento estiver a esquerda do switch_2
            if(game->switch_2->x < game->switch_1->x) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, (x * 64) + c, y * 64, 64, 64, 0);
            // se elemento estiver a cima do switch_2
            if(game->switch_2->y > game->switch_1->y) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, x * 64, (y * 64) - c, 64, 64, 0);
            // se elemento estiver em baixo do switch_2
            if(game->switch_2->y < game->switch_1->y) 
                al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, x * 64, (y * 64) + c, 64, 64, 0);
        } else al_draw_scaled_bitmap(sprite_x, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);

    }

}

void render_switch(GAMEstructure *game, AllegroStructure *allegro, int c) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            draw_switch(game->map[j][i], i, j, allegro, game, c);
        }
    }
    al_draw_scaled_bitmap(allegro->sprite_exit, 0, 0, 32, 32, 400, 536, 64, 64, 0);
    al_draw_textf(allegro->font, al_map_rgb(255, 255, 255), 210, 550, 0, "Time: %.1f", game->time);
    al_draw_textf(allegro->font, al_map_rgb(255, 255, 255), 10, 550, 0, "Score: %i", game->score);
    al_flip_display();
}

void must_init(bool test, const char *description) {
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void render(GAMEstructure *game, AllegroStructure *allegro) {
    if(allegro->flash) {
        if(allegro->flash_c == 0) al_clear_to_color(al_map_rgb(255, 51, 51));
        if(allegro->flash_c == 1) al_clear_to_color(al_map_rgb(102, 204, 0));
        if(allegro->flash_c == 2) al_clear_to_color(al_map_rgb(0, 102, 204));
        if(allegro->flash_c == 3) al_clear_to_color(al_map_rgb(255, 0, 127));
        if(allegro->flash_c == 4) al_clear_to_color(al_map_rgb(255, 128, 0));
        if(allegro->flash_c == 5) allegro->flash_c = 0;
        else allegro->flash_c++;
    } else al_clear_to_color(al_map_rgb(0, 0, 0));

    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            draw(game->map[j][i], i, j, allegro);
        }
    }
    al_draw_scaled_bitmap(allegro->sprite_exit, 0, 0, 32, 32, 400, 536, 64, 64, 0);
    al_draw_textf(allegro->font, al_map_rgb(255, 255, 255), 210, 550, 0, "Time: %.1f", game->time);
    al_draw_textf(allegro->font, al_map_rgb(255, 255, 255), 10, 550, 0, "Score: %i", game->score);
    al_flip_display();
}

void draw(int element, int y, int x, AllegroStructure *allegro) {
    // al_draw_scaled_bitmap(sprites, cor_x_sprite, cor_y_sprite, tamx, tamy, cor_x_allgr, cor_y_allgr, tamx, tamy, 0)
    if(element >= BEAMX && element < BEAMXLIMIT) {
        al_draw_scaled_bitmap(allegro->sprite_beam_x, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
    } else if(element >= BEAMY && element < BEAMYLIMIT) {
        al_draw_scaled_bitmap(allegro->sprite_beam_y, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
    } else {
        switch (element) {
            case 0:
                al_draw_scaled_bitmap(allegro->sprite_1, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 1:
                al_draw_scaled_bitmap(allegro->sprite_2, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 2:
                al_draw_scaled_bitmap(allegro->sprite_3, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 3:
                al_draw_scaled_bitmap(allegro->sprite_4, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 4:
                al_draw_scaled_bitmap(allegro->sprite_5, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 5:
                al_draw_scaled_bitmap(allegro->sprite_6, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 10:
                al_draw_scaled_bitmap(allegro->sprite_12, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 11:
                al_draw_scaled_bitmap(allegro->sprite_22, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 12:
                al_draw_scaled_bitmap(allegro->sprite_32, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 13:
                al_draw_scaled_bitmap(allegro->sprite_42, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 14:
                al_draw_scaled_bitmap(allegro->sprite_52, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
            case 15:
                al_draw_scaled_bitmap(allegro->sprite_62, 0, 0, 64, 64, x * 64, y * 64, 64, 64, 0);
                break;
        }
    }

}

void init_allegro(AllegroStructure *allegro) {

    int d_height = 512;
    int d_width = 600;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_audio(), "sound"); 
    must_init(al_init_acodec_addon(), "addon"); 
    must_init(al_init_image_addon(), "image");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_font_addon(), "font_addon");
    must_init(al_init_ttf_addon(), "ttf_addon");
    al_reserve_samples(1);

    allegro->redraw = true;

    allegro->font = al_load_font("./fonts/Bebas-Regular.ttf", 40, 0);
    must_init(allegro->font, "font");

    allegro->sprite_1 = al_load_bitmap("./sprites_64/0001.png");
    must_init(allegro->sprite_1, "sprite_1");

    allegro->sprite_2 = al_load_bitmap("./sprites_64/0002.png");
    must_init(allegro->sprite_2, "sprite_2");

    allegro->sprite_3 = al_load_bitmap("./sprites_64/0003.png");
    must_init(allegro->sprite_3, "sprite_3");

    allegro->sprite_4 = al_load_bitmap("./sprites_64/0004.png");
    must_init(allegro->sprite_4, "sprite_4");

    allegro->sprite_5 = al_load_bitmap("./sprites_64/0005.png");
    must_init(allegro->sprite_5, "sprite_5");

    allegro->sprite_6 = al_load_bitmap("./sprites_64/0006.png");
    must_init(allegro->sprite_6, "sprite_6");

    allegro->sprite_12 = al_load_bitmap("./sprites_64/0001-2.png");
    must_init(allegro->sprite_12, "sprite_12");

    allegro->sprite_22 = al_load_bitmap("./sprites_64/0002-2.png");
    must_init(allegro->sprite_22, "sprite_22");

    allegro->sprite_32 = al_load_bitmap("./sprites_64/0003-2.png");
    must_init(allegro->sprite_32, "sprite_32");

    allegro->sprite_42 = al_load_bitmap("./sprites_64/0004-2.png");
    must_init(allegro->sprite_42, "sprite_42");

    allegro->sprite_52 = al_load_bitmap("./sprites_64/0005-2.png");
    must_init(allegro->sprite_52, "sprite_52");

    allegro->sprite_62 = al_load_bitmap("./sprites_64/0006-2.png");
    must_init(allegro->sprite_62, "sprite_62");

    allegro->sprite_beam_y = al_load_bitmap("./sprites_64/beam-y.png");
    must_init(allegro->sprite_beam_y, "sprite_beam_y");

    allegro->sprite_beam_x = al_load_bitmap("./sprites_64/beam-x.png");
    must_init(allegro->sprite_beam_x, "sprite_beam_x");

    allegro->sprite_exit = al_load_bitmap("./sprites_64/exit.png");
    must_init(allegro->sprite_exit, "sprite_exit");

    allegro->sample = al_load_sample("./samples/pop.opus");
    must_init(allegro->sample, "samples 3");

    allegro->timer = al_create_timer(1.0 / 10.0);
    must_init(allegro->timer, "timer");

    allegro->tick = al_create_timer(1.0 / 15.0);
    must_init(allegro->tick, "tick");

    allegro->gravity_tick = al_create_timer(1.0 / 8.0);
    must_init(allegro->gravity_tick, "gravity_tick");
    
    allegro->queue = al_create_event_queue();
    must_init(allegro->queue, "queue");

    allegro->disp = al_create_display(d_height, d_width);
    must_init(allegro->disp, "display");

    al_register_event_source(allegro->queue, al_get_keyboard_event_source());
    al_register_event_source(allegro->queue, al_get_mouse_event_source());
    al_register_event_source(allegro->queue, al_get_display_event_source(allegro->disp));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->timer));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->tick));
    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->gravity_tick));

    al_start_timer(allegro->timer);
    al_start_timer(allegro->tick);
    al_start_timer(allegro->gravity_tick);

    allegro->flash = false;
    allegro->flash_c = 0;
}