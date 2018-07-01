#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "sprite.h"

SDL_Color background_color = {0, 0, 0, 0};

#ifdef __WIN32__
  int WinMain(int argc, char *argv[])
#else
  int main(int argc, char *argv[])
#endif
{
  /* init SDL start */
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    exit(EXIT_FAILURE);
  atexit(SDL_Quit);
  SDL_WM_SetCaption("SDL Demo", NULL);
  
  SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  if(!screen)
    exit(EXIT_FAILURE);
  /* init SDL end */
  
  /* init sprites */
  if(!sprite_init())
    exit(EXIT_FAILURE);
  atexit(sprite_quit);
  
  /* init SDL mixer */
  if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 256) < 0)
    exit(EXIT_FAILURE);
  atexit(Mix_CloseAudio);
  
  /* load music */
  Mix_Music *keygen_music = Mix_LoadMUS("keygen_music.flac");
  if(!keygen_music)
    exit(EXIT_FAILURE);
  Mix_PlayMusic(keygen_music, 1);
  
  /* create first sprite */
  new_sprite *first_sprite = create_sprite(screen->w/2 - SPRITE_W/2, screen->h/2 - SPRITE_H/2);
  first_sprite->src.x = SPRITE_W * 12;
  first_sprite->src.y = SPRITE_H * 4;
  
  /* main loop start */
  SDL_Event user_event;
  Uint32 time_stop = SDL_GetTicks();
  while(user_event.type != SDL_QUIT)
  {
    /* poll events */
    while(SDL_PollEvent(&user_event))
    {
      /* break out of event polling loop to prevent change of event type due to following events */
      if(user_event.type == SDL_QUIT)
        break;
    }
    
    /* animate */
    if(SDL_GetTicks() - time_stop > 1900 &&
       SDL_GetTicks() - time_stop < 2150)
    {
      clone_first_sprite();
    }
    else if(SDL_GetTicks() - time_stop > 3700 &&
            SDL_GetTicks() - time_stop < 3950)
    {
      clone_first_sprite_generation();
    }
    else if(SDL_GetTicks() - time_stop > 5600 &&
            SDL_GetTicks() - time_stop < 5850)
    {
      clone_second_sprite_generation();
      build_wall();
    }
    else if(SDL_GetTicks() - time_stop > 5850 &&
            SDL_GetTicks() - time_stop < 7300)
    {
      build_wall();
    }
    else if(SDL_GetTicks() - time_stop > 7300 &&
            SDL_GetTicks() - time_stop < 9300)
    {
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 9300 &&
            SDL_GetTicks() - time_stop < 11300)
    {
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 11300 &&
            SDL_GetTicks() - time_stop < 13100)
    {
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 13100 &&
            SDL_GetTicks() - time_stop < 14100)
    {
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 14100 &&
            SDL_GetTicks() - time_stop < 18100)
    {
      flash_background(250);
      rain_generation(0);
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 18100 &&
            SDL_GetTicks() - time_stop < 20000)
    {
      background_color.r = 50;
      background_color.g = 50;
      background_color.b = 50;
      rain_generation(1);
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 20000 &&
            SDL_GetTicks() - time_stop < 22400)
    {
      update_first_sprites_time();
      rain_generation(2);
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 22400 &&
            SDL_GetTicks() - time_stop < 22600)
    {
      rain_generation_reverse(1);
      update_first_sprites_time();
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 22600 &&
            SDL_GetTicks() - time_stop < 23000)
    {
      rain_generation_reverse(1);
      update_second_sprites_time();
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      
      second_sprite_generation_dance();
      second_sprite_generation_walk();
    }
    else if(SDL_GetTicks() - time_stop > 23000 &&
            SDL_GetTicks() - time_stop < 23400)
    {
      update_clone_sprites_time();
      first_sprite_dance();
      clone_generation_dance();
      rain_generation_reverse(2);
      
      first_sprite_generation_dance();
      first_sprite_generation_walk();
      
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 23400 &&
            SDL_GetTicks() - time_stop < 23800)
    {
      first_sprite_dance();
      
      clone_generation_dance();
      clone_generation_walk();
      
      rain_generation_reverse(0);
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 23800 &&
            SDL_GetTicks() - time_stop < 29000)
    {
      first_sprite_dance();
      clone_generation_dance();
      rain_generation_reverse(1);
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 29000 &&
            SDL_GetTicks() - time_stop < 37000)
    {
      rain_generation_reverse(2);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 37000 &&
            SDL_GetTicks() - time_stop < 45000)
    {
      rain_generation_reverse(0);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 45000 &&
            SDL_GetTicks() - time_stop < 55000)
    {
      rain_generation_reverse(1);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 55000 &&
            SDL_GetTicks() - time_stop < 58000)
    {
      rain_generation_reverse(0);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 58000 &&
            SDL_GetTicks() - time_stop < 60000)
    {
      rain_generation_reverse(3);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 60000 &&
            SDL_GetTicks() - time_stop < 97000)
    {
      rain_generation_reverse(3);
      background_color.r = 0;
      background_color.g = 0;
      background_color.b = 0;
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 97000 &&
            SDL_GetTicks() - time_stop < 110000)
    {
      rain_generation_reverse(4);
      background_color.r = 50;
      background_color.g = 50;
      background_color.b = 50;
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    else if(SDL_GetTicks() - time_stop > 110000 &&
            SDL_GetTicks() - time_stop < 116800)
    {
      rain_generation_reverse(1);
      
      first_sprite_dance();
      clone_generation_dance();
      first_sprite_generation_dance();
      second_sprite_generation_dance();
    }
    
    /* clear screen */
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, background_color.r, background_color.g, background_color.b));
    
    /* draw all sprites */
    blit_all_sprites(screen);
    
    /* flip screen */
    SDL_Flip(screen);
  }
  /* main loop end */
  
  Mix_FreeMusic(keygen_music);
  return EXIT_SUCCESS;
}