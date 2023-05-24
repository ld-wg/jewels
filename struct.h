#ifndef STRUCT_H
#define STRUCT_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>

#define MAPSIZE 8
#define BEAMY 100
#define BEAMYLIMIT 105
#define BEAMX 200
#define BEAMXLIMIT 205
#define LINESIZE 128

typedef struct coord {
    int x;
    int y;
} coord;

typedef struct GAMEstructure {
    int **map;
    bool is_falling;
    struct coord *switch_1;
    struct coord *switch_2;
    int score;
    float time;
    FILE *arq;
} GAMEstructure;

typedef struct TempBitMap {
    ALLEGRO_BITMAP *sprite_x;
} TempBitMap;

typedef struct AllegroStructure {
    ALLEGRO_BITMAP *sprite_1;
    ALLEGRO_BITMAP *sprite_2;
    ALLEGRO_BITMAP *sprite_3;
    ALLEGRO_BITMAP *sprite_4;
    ALLEGRO_BITMAP *sprite_5;
    ALLEGRO_BITMAP *sprite_6;
    ALLEGRO_BITMAP *sprite_12;
    ALLEGRO_BITMAP *sprite_22;
    ALLEGRO_BITMAP *sprite_32;
    ALLEGRO_BITMAP *sprite_42;
    ALLEGRO_BITMAP *sprite_52;
    ALLEGRO_BITMAP *sprite_62;
    ALLEGRO_BITMAP *sprite_beam_y;
    ALLEGRO_BITMAP *sprite_beam_x;
    ALLEGRO_BITMAP *sprite_exit;
    ALLEGRO_SAMPLE *sample;
    ALLEGRO_TIMER *timer;
    ALLEGRO_TIMER *tick;
    ALLEGRO_TIMER *gravity_tick;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_FONT *font;
    ALLEGRO_EVENT event;
    bool flash;
    int flash_c;
    bool redraw;
    int pos_x;
    int pos_y;
    unsigned char key[ALLEGRO_KEY_MAX];
} AllegroStructure;

#endif