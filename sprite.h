#ifndef SPRITE_H_GUARD
#define SPRITE_H_GUARD

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <unistd.h>

#define SPRITE_ZOOM 2
#define SPRITE_W 16 * SPRITE_ZOOM
#define SPRITE_H 16 * SPRITE_ZOOM
#define SPRITE_SPEED_FLOAT 6.0

typedef struct new_sprite{
  double x, y;
  Uint32 move_time_stop;
  
  SDL_Rect src;
  
  struct new_sprite *next;
  struct new_sprite *prev;
}new_sprite;

int sprite_init(void);
void sprite_quit(void);
void free_all_sprites(void);

new_sprite *create_sprite(const int x, const int y);
void free_sprite(new_sprite *sprite);
void blit_all_sprites(SDL_Surface *dst);

void update_first_sprites_time(void);
void update_second_sprites_time(void);
void update_clone_sprites_time(void);

/* crappy animation functions */
void clone_first_sprite(void);
void clone_first_sprite_generation(void);
void clone_second_sprite_generation(void);

void first_sprite_dance(void);

void clone_generation_dance(void);
void clone_generation_walk(void);

void first_sprite_generation_dance(void);
void first_sprite_generation_walk(void);

void second_sprite_generation_dance(void);
void second_sprite_generation_walk(void);

void build_wall(void);
void rain_generation(const int mode);
void rain_generation_reverse(const int mode);
void flash_background(const Uint32 time);

#endif