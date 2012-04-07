#include <ctime>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>

#include "blockstar.hpp"
#include "utility.hpp"

using namespace std;


int main ()
{
  srand (time(NULL));
  
  SDL_Init (SDL_INIT_VIDEO);
  atexit (SDL_Quit);
  
  game::screen = SDL_SetVideoMode(settings::window_h, 
                                  settings::window_w, 
                                  settings::window_bpp, 
                                  SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  SDL_FillRect(game::screen, NULL, settings::background_color);
  SDL_WM_SetCaption (settings::window_title, 0);
  
  game::block = SDL_CreateRGBSurface(SDL_SWSURFACE, 
                                     settings::square_size, 
                                     settings::square_size, 
                                     settings::window_bpp, 
                                     0, 0, 0, 0);
                                     
  TTF_Init();
  
  game::text = TTF_OpenFont ("joystix.ttf", settings::text_size);
  game::state = STATE_MENU;
   
  display_menu();
     
  event_loop();
  
  exit(0);
}
