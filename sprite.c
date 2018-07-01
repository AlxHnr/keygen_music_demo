#include "sprite.h"

static new_sprite *first_sprite = NULL;
static new_sprite *last_sprite = NULL;

static new_sprite *sprite_nr_9 = NULL;
static new_sprite *sprite_nr_12 = NULL;
static new_sprite *sprite_rain = NULL;

static SDL_Surface *sprite_sheet = NULL;

static Uint32 dance_time_stop = 0;
static Uint32 block_time_stop = 0;
static Uint32 rain_time_stop = 0;
static Uint32 background_time_stop = 0;

extern SDL_Color background_color;

int sprite_init(void)
{
  SDL_Surface *cache_surface = IMG_Load("rpg.png");
  if(!cache_surface)
    return 0;
  
  sprite_sheet = zoomSurface(cache_surface, SPRITE_ZOOM, SPRITE_ZOOM, SMOOTHING_ON);
  SDL_FreeSurface(cache_surface);
  
  return 1;
}
void sprite_quit(void)
{
  SDL_FreeSurface(sprite_sheet);
  
  free_all_sprites();
}
void free_all_sprites(void)
{
  while(first_sprite)
    free_sprite(first_sprite);
}

new_sprite *create_sprite(const int x, const int y)
{
  /* allocate memory and set new sprite to last_sprite */
  if(!first_sprite)
  {
    first_sprite = malloc(sizeof(struct new_sprite));
    if(!first_sprite)
      return NULL;
    
    first_sprite->next = NULL;
    first_sprite->prev = NULL;
    last_sprite = first_sprite;
  }
  else
  {
    last_sprite->next = malloc(sizeof(struct new_sprite));
    if(!last_sprite->next)
      return NULL;
    
    last_sprite->next->prev = last_sprite;
    last_sprite->next->next = NULL;
    last_sprite = last_sprite->next;
  }
  
  last_sprite->x = x;
  last_sprite->y = y;
  
  last_sprite->move_time_stop = SDL_GetTicks();
  
  last_sprite->src.x = 0;
  last_sprite->src.y = 0;
  last_sprite->src.w = SPRITE_W;
  last_sprite->src.h = SPRITE_H;
  
  return last_sprite;
}
void free_sprite(new_sprite *sprite)
{
  if(!sprite)
    return;
  
  if(sprite == first_sprite)
  {
    first_sprite = sprite->next;
    
    if(sprite->next)
      sprite->next->prev = NULL;
    else
      last_sprite = NULL;
  }
  else if(sprite == last_sprite)
  {
    sprite->prev->next = NULL;
    last_sprite = sprite->prev;
  }
  else
  {
    sprite->next->prev = sprite->prev;
    sprite->prev->next = sprite->next;
  }
  
  free(sprite);
}
void blit_all_sprites(SDL_Surface *dst)
{
  new_sprite *sprite = NULL;
  SDL_Rect dst_rect;
  
  for(sprite = last_sprite; sprite; sprite = sprite->prev)
  {
    dst_rect.x = sprite->x;
    dst_rect.y = sprite->y;
    
    SDL_BlitSurface(sprite_sheet, &sprite->src, dst, &dst_rect);
  }
}
void update_first_sprites_time(void)
{
  new_sprite *sprite = NULL;
  
  /* update all generation timestops */
  for(sprite = first_sprite; sprite != sprite_rain->next; sprite = sprite->next)
      sprite->move_time_stop = SDL_GetTicks();
}
void update_second_sprites_time(void)
{
  new_sprite *sprite = NULL;
  
  /* update all generation timestops */
  for(sprite = first_sprite; sprite != sprite_nr_12->next; sprite = sprite->next)
      sprite->move_time_stop = SDL_GetTicks();
}
void update_clone_sprites_time(void)
{
  new_sprite *sprite = NULL;
  
  /* update all generation timestops */
  for(sprite = first_sprite; sprite != sprite_nr_9->next; sprite = sprite->next)
      sprite->move_time_stop = SDL_GetTicks();
}

/* crappy animation functions */
void clone_first_sprite(void)
{
  new_sprite *sprite = NULL;
  int counter;
  
  /* create 8 sprites if they dont exist */
  if(!first_sprite->next)
  {
    /* set first sprite movement timestop */
    first_sprite->move_time_stop = SDL_GetTicks();
    
    for(counter = 0; counter < 8; counter++)
    {
      /* sprite_nr_9 points to the last sprite createt by this function */
      sprite_nr_9 = create_sprite(first_sprite->x, first_sprite->y);
    }
  }
  else
  {
    sprite = first_sprite->next;
    
    /* move them out from center */
    for(counter = 0; counter < 8; counter++)
    {
      switch(counter)
      {
        case 0:
          sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 1:
          sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 2:
          sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 3:
          sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 4:
          sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 5:
          sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 6:
          sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
        case 7:
          sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
          break;
      }
      
      sprite->move_time_stop = SDL_GetTicks();
      sprite = sprite->next;
    }
  }
}
void clone_first_sprite_generation(void)
{
  new_sprite *sprite = NULL;
  new_sprite *cache_sprite = NULL;
  int counter;
  
  /* create next 12 sprites if they dont exist */
  if(!sprite_nr_9->next)
  {
    /* update first sprite generation movement timestop and add new sprites */
    for(sprite = first_sprite; sprite; sprite = sprite->next)
      sprite->move_time_stop = SDL_GetTicks();
    
    /* create some new sprites */
    counter = 0;
    sprite = first_sprite->next;
    while(counter < 8)
    {
      /* create one extra sprite if its on a edge position */
      if((counter == 0) || (counter == 2) || (counter == 5) || (counter == 7))
      {
        cache_sprite = create_sprite(sprite->x, sprite->y);
        cache_sprite->src.x = SPRITE_W * 4;
      }
      
      cache_sprite = create_sprite(sprite->x, sprite->y);
      cache_sprite->src.x = SPRITE_W * 4;
      
      sprite_nr_12 = cache_sprite;
      
      sprite = sprite->next;
      counter++;
    }
  }
  else
  {
    counter = 0;
    sprite = sprite_nr_9->next;
    while(counter < 12)
    {
      if((counter == 0) || (counter == 2) || (counter == 3))
      {
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if((counter == 1) || (counter == 5) || (counter == 7))
      {
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if((counter == 4) || (counter == 6) || (counter == 10))
      {
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if((counter == 8) || (counter == 9) || (counter == 11))
      {
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      
      sprite->move_time_stop = SDL_GetTicks();
      sprite = sprite->next;
      counter++;
    }
  }
}
void clone_second_sprite_generation(void)
{
  new_sprite *sprite = NULL;
  new_sprite *cache_sprite = NULL;
  int counter;
  
  /* create next 4 sprites if they dont exist */
  if(!sprite_nr_12->next)
  {
    /* update first sprite generation movement timestop and add new sprites */
    for(sprite = first_sprite; sprite; sprite = sprite->next)
      sprite->move_time_stop = SDL_GetTicks();
    
    /* create some new sprites */
    counter = 0;
    sprite = sprite_nr_9->next;
    while(counter < 12)
    {
      /* clone only sprites in center */
      if((counter == 2) || (counter == 5) || (counter == 6) || (counter == 9))
      {
        cache_sprite = create_sprite(sprite->x, sprite->y);
        cache_sprite->src.x = SPRITE_W * 12;
        
        sprite_rain = cache_sprite;
      }
      
      sprite = sprite->next;
      counter++;
    }
  }
  else
  {
    counter = 0;
    sprite = sprite_nr_12->next;
    while(counter < 4)
    {
      if(counter == 0)
      {
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if(counter == 1)
      {
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if(counter == 2)
      {
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      else if(counter == 3)
      {
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
      }
      
      sprite->move_time_stop = SDL_GetTicks();
      sprite = sprite->next;
      counter++;
    }
  }
}

void first_sprite_dance(void)
{
  if(SDL_GetTicks() - dance_time_stop > 950)
    first_sprite->src.x = SPRITE_W * 12;
  else if(SDL_GetTicks() - dance_time_stop > 700)
    first_sprite->src.x = SPRITE_W * 13;
  else if(SDL_GetTicks() - dance_time_stop > 450)
    first_sprite->src.x = SPRITE_W * 12;
  else if(SDL_GetTicks() - dance_time_stop > 200)
    first_sprite->src.x = SPRITE_W * 14;
}
void clone_generation_dance(void)
{
  int counter = 0;
  new_sprite *sprite = first_sprite->next;
  while(counter < 8)
  {
    if(SDL_GetTicks() - dance_time_stop > 950)
      sprite->src.x = 0;
    else if(SDL_GetTicks() - dance_time_stop > 700)
      sprite->src.x = SPRITE_W;
    else if(SDL_GetTicks() - dance_time_stop > 450)
      sprite->src.x = 0;
    else if(SDL_GetTicks() - dance_time_stop > 200)
      sprite->src.x = SPRITE_W * 2;
    
    sprite = sprite->next;
    counter++;
  }
}
void clone_generation_walk(void)
{
  int counter = 0;
  new_sprite *sprite = first_sprite->next;
  while(counter < 8)
  {
    switch(counter)
    {
      case 0:
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 1:
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 2:
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 3:
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 4:
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 5:
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 6:
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 7:
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
    }
    
    sprite->move_time_stop = SDL_GetTicks();
    sprite = sprite->next;
    counter++;
  }
}

void first_sprite_generation_dance(void)
{
  int counter = 0;
  new_sprite *sprite = sprite_nr_9->next;
  while(counter < 12)
  {
    if(SDL_GetTicks() - dance_time_stop > 950)
      sprite->src.x = SPRITE_W * 4;
    else if(SDL_GetTicks() - dance_time_stop > 700)
      sprite->src.x = SPRITE_W * 5;
    else if(SDL_GetTicks() - dance_time_stop > 450)
      sprite->src.x = SPRITE_W * 4;
    else if(SDL_GetTicks() - dance_time_stop > 200)
      sprite->src.x = SPRITE_W * 6;
    
    sprite = sprite->next;
    counter++;
  }
}
void first_sprite_generation_walk(void)
{
  int counter = 0;
  new_sprite *sprite = sprite_nr_9->next;
  while(counter < 12)
  {
    if((counter == 0) || (counter == 2) || (counter == 3))
      sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    else if((counter == 1) || (counter == 5) || (counter == 7))
      sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    else if((counter == 4) || (counter == 6) || (counter == 10))
      sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    else if((counter == 8) || (counter == 9) || (counter == 11))
      sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    
    sprite->move_time_stop = SDL_GetTicks();
    sprite = sprite->next;
    counter++;
  }
}

void second_sprite_generation_dance(void)
{
  int counter = 0;
  new_sprite *sprite = sprite_nr_12->next;
  while(counter < 4)
  {
    if(SDL_GetTicks() - dance_time_stop > 950)
    {
      /* reset only timer if every sprite has set frame */
      if(counter == 3)
        dance_time_stop = SDL_GetTicks();
      
      sprite->src.x = SPRITE_W * 12;
    }
    else if(SDL_GetTicks() - dance_time_stop > 700)
      sprite->src.x = SPRITE_W * 13;
    else if(SDL_GetTicks() - dance_time_stop > 450)
      sprite->src.x = SPRITE_W * 12;
    else if(SDL_GetTicks() - dance_time_stop > 200)
      sprite->src.x = SPRITE_W * 14;
    
    sprite = sprite->next;
    counter++;
  }
}
void second_sprite_generation_walk(void)
{
  int counter = 0;
  new_sprite *sprite = sprite_nr_12->next;
  while(counter < 4)
  {
    switch(counter)
    {
      case 0:
        sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 1:
        sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 2:
        sprite->x += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
      case 3:
        sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
        break;
    }
    
    sprite->move_time_stop = SDL_GetTicks();
    sprite = sprite->next;
    counter++;
  }
}

void build_wall(void)
{
  static int x = 0;
  static int y = 0;
  static int x_movement = SPRITE_W;
  static int y_movement = 0;
  
  SDL_Surface *dst = SDL_GetVideoSurface();
  new_sprite *cache_sprite = NULL;
  
  if(SDL_GetTicks() - block_time_stop > 3)
  {
    if(!(y == dst->h - SPRITE_H && x > dst->w))
    {
      cache_sprite = create_sprite(x, y);
      cache_sprite->src.x = SPRITE_W * 15;
      cache_sprite->src.y = SPRITE_H * 7;
      
      if(x > dst->w && y == 0)
      {
        x = 0;
        y = 0;
        x_movement = 0;
        y_movement = SPRITE_H;
      }
      else if(x == 0 && y > dst->h)
      {
        x = dst->w - SPRITE_W;
        y = 0;
      }
      else if(x == dst->w - SPRITE_W && y > dst->h)
      {
        x = 0;
        y = dst->h - SPRITE_H;
        x_movement = SPRITE_W;
        y_movement = 0;
      }
      
      x += x_movement;
      y += y_movement;
    }
    else
    {
      if(sprite_nr_12->next->next->next->next->next)
        free_sprite(sprite_nr_12->next->next->next->next->next);
    }
    
    block_time_stop = SDL_GetTicks();
  }
}
void rain_generation(const int mode)
{
  SDL_Surface *dst = SDL_GetVideoSurface();
  new_sprite *sprite = NULL;
  new_sprite *cache_sprite = NULL;
  int counter;
  
  /* create every few seconds a new sprite row */
  if(SDL_GetTicks() - rain_time_stop > 250)
  {
    for(counter = 0; counter < 50; counter++)
    {
      cache_sprite = create_sprite(counter * SPRITE_W + SPRITE_W, -SPRITE_H);
      
      if(mode == 1)
        cache_sprite->src.x = SPRITE_W * (rand() % 2 + 13);
      else if(mode == 2)
        cache_sprite->src.x = SPRITE_W * 13;
      else if(mode == 3)
        cache_sprite->src.x = SPRITE_W * 12;
      else if(mode == 4)
        cache_sprite->src.x = SPRITE_W * (rand() % 3 + 12);
      else
        cache_sprite->src.x = SPRITE_W * 14;
        
      cache_sprite->src.y = SPRITE_H * 7;
      
      counter++;
    }
    
    rain_time_stop = SDL_GetTicks();
  }
  
  /* animate them */
  for(sprite = sprite_rain->next; sprite; sprite = sprite->next)
  {
    if((sprite->x < -SPRITE_W) || (sprite->y > dst->h))
    {
      free_sprite(sprite);
      return;
    }
    else
    {
      sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT/2;
      sprite->y += (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    }
    
    sprite->move_time_stop = SDL_GetTicks();
  }
}
void rain_generation_reverse(const int mode)
{
  SDL_Surface *dst = SDL_GetVideoSurface();
  new_sprite *sprite = NULL;
  new_sprite *cache_sprite = NULL;
  int counter;
  
  /* create every few seconds a new sprite row */
  if(SDL_GetTicks() - rain_time_stop > 250)
  {
    for(counter = 0; counter < 35; counter++)
    {
      cache_sprite = create_sprite(counter * SPRITE_W + SPRITE_W, dst->h + SPRITE_H);
      
      if(mode == 1)
        cache_sprite->src.x = SPRITE_W * (rand() % 2 + 13);
      else if(mode == 2)
        cache_sprite->src.x = SPRITE_W * 13;
      else if(mode == 3)
        cache_sprite->src.x = SPRITE_W * 12;
      else if(mode == 4)
        cache_sprite->src.x = SPRITE_W * (rand() % 3 + 12);
      else
        cache_sprite->src.x = SPRITE_W * 14;
      
      cache_sprite->src.y = SPRITE_H * 7;
      
      counter++;
    }
    
    rain_time_stop = SDL_GetTicks();
  }
  
  /* animate them */
  for(sprite = sprite_rain->next; sprite; sprite = sprite->next)
  {
    if((sprite->x < -SPRITE_W) || (sprite->y < -SPRITE_H))
    {
      free_sprite(sprite);
      return;
    }
    else
    {
      sprite->x -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT/2;
      sprite->y -= (float)(SDL_GetTicks() - sprite->move_time_stop)/SPRITE_SPEED_FLOAT;
    }
    
    sprite->move_time_stop = SDL_GetTicks();
  }
}
void flash_background(const Uint32 time)
{
  extern SDL_Color background_color;
  
  if(SDL_GetTicks() - background_time_stop > time)
  {
    background_color.r = rand() % 150;
    background_color.g = rand() % 150;
    background_color.b = rand() % 150;
    
    background_time_stop = SDL_GetTicks();
  }
}