#include <fstream>
#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>

#include "blockstar.hpp"
#include "utility.hpp"

using namespace std;

namespace game
{
  SDL_Surface *screen;
  SDL_Surface *block;
  TTF_Font *text;
  
  struct shape_t temp_tet;
  struct shape_t curr_tet;
  struct shape_t next_tet;
  
  struct score_ds temp_high;
  struct score_ds high_scores[10];
  struct score_ds score_save;
 
  state_e from_state;
  state_e state;
  state_e to_state;  
  
  int score;
  int level;
  int lines;
  int speed;
  int board[10][20];
}


void display_save_score()
{
  char text_buffer[80];
  
  TTF_Font *txt_font = TTF_OpenFont("joystix.ttf", settings::square_size / 2);
  SDL_Surface *text_image;
  SDL_Rect dest;
  SDL_Color text_color = { 255, 0, 255, 0 };
  
  dest.x = settings::square_size * 2;
  dest.y = settings::square_size * 7;
  dest.w = settings::square_size * 16;
  dest.h = settings::square_size * 7;
  SDL_FillRect (game::screen, &dest, settings::background_color);
  
  sprintf (text_buffer, "GAME OVER");
  text_image = TTF_RenderText_Solid (txt_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 8;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  
  sprintf (text_buffer, "Type your initials then hit enter:");
  text_image = TTF_RenderText_Solid (txt_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 10;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  
  sprintf (text_buffer, "%s", game::score_save.initials.c_str() );
  text_image = TTF_RenderText_Solid (txt_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 12;
  dest.w = text_image->w;
  dest.h = text_image->h;
  
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  SDL_Flip (game::screen);
}

void display_pause()
{
  char text_buffer[80];
  
  TTF_Font *text_font = TTF_OpenFont ("joystix.ttf", settings::square_size);
  SDL_Surface *text_image;
  SDL_Rect dest;
  SDL_Color text_color = { 255, 0, 255, 0 };
  
  dest.x = settings::square_size * 4;
  dest.y = settings::square_size * 9;
  dest.w = settings::square_size * 12;
  dest.h = settings::square_size * 5;
  SDL_FillRect (game::screen, &dest, settings::background_color);
  
  sprintf (text_buffer, "GAME PAUSED");
  text_image = TTF_RenderText_Solid (text_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 10;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  
  sprintf (text_buffer, "(R)esume");
  text_image = TTF_RenderText_Solid (text_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 12;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  SDL_Flip (game::screen);
}

void verify_quit()
{
  char text_buffer[80];
  TTF_Font *text_font = TTF_OpenFont ("joystix.ttf", 
                                      settings::square_size / 2 +
                                      settings::square_size / 4);
  SDL_Surface *text_image;
  SDL_Rect dest;
  SDL_Color text_color = { 255, 0, 255, 0 };
  
  dest.x = settings::square_size * 2;
  dest.y = settings::square_size * 9;
  dest.w = settings::square_size * 16;
  dest.h = settings::square_size * 5;
  SDL_FillRect (game::screen, &dest, settings::background_color);
  
  sprintf (text_buffer, "Are you sure you want to quit?");
  text_image = TTF_RenderText_Solid (text_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 10;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  
  sprintf (text_buffer, "(Y)es        (N)o");
  text_image = TTF_RenderText_Solid (text_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 12;
  dest.w = text_image->w;
  dest.h = text_image->h;
  
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  SDL_Flip (game::screen);
}

void draw_commands (bool show_pause)
{
  char text_buffer[80];
  TTF_Font *text_font = TTF_OpenFont ("joystix.ttf", settings::square_size/2);
  SDL_Rect dest;
  SDL_Surface *text_image;
  SDL_Color text_color =
  {
    (uint8_t)(settings::text_color >> 16),
    (uint8_t)(settings::text_color >>  8),
    (uint8_t)(settings::text_color >>  0),
    0
  };
  
  dest.x = 0;
  dest.y = settings::square_size * 22;
  dest.w = settings::square_size * 20;
  dest.h = settings::square_size;
  SDL_FillRect (game::screen, &dest, settings::background_color);

  if (show_pause)
    sprintf (text_buffer, "(M)enu      (Q)uit      (P)ause");
    
  else
    sprintf (text_buffer, "(M)enu          (Q)uit");

  text_image = TTF_RenderText_Solid (text_font, text_buffer, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 22 + (settings::square_size - text_image->h) / 2;
  dest.w = text_image->w;
  dest.h = text_image->h;
  
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  SDL_Flip (game::screen);
}

void display_scores()
{
  bool swapped;
  int i;
  int fill_score;
  int num_scores = 0;
  
  string str;
  string initials;
  string score;
  string fill_name;
  
  char text_buffer[80];
  ifstream file;
  
  file.open ("scores.dat", ios::in);
  
  TTF_Font *hsFont = TTF_OpenFont ("joystix.ttf", settings::square_size/2+settings::square_size/4);
  SDL_Rect dest;
  SDL_Surface *text_image;
  SDL_Color text_color = { 0, 255, 255, 0 };
  SDL_FillRect (game::screen, NULL, settings::background_color);

  if (!file.is_open() )
    num_scores = 0;
    
  else
  {
    while (getline (file, str) )
    {
      if (str.empty() )
        continue;

      game::high_scores[num_scores].initials = get_token (str, 0, ":");
      game::high_scores[num_scores].score = atoi (get_token (str, 1, ":").c_str() );
      ++num_scores;
    }

    if (num_scores > 1)   // bubble sort
    {
      for (swapped = true; swapped; )
      {
        swapped = false;

        for (i = 0; i < num_scores - 1; ++i)
          if (game::high_scores[i].score < game::high_scores[i+1].score)
          {
            game::temp_high = game::high_scores[i+1];
            game::high_scores[i+1] =  game::high_scores[i];
            game::high_scores[i] = game::temp_high;
            swapped = true;
          }
      }
    }
  }

  for (i = 0; i < 10; ++i)
  {
    if (i>=num_scores)
    {
      fill_name = "---";
      fill_score = 0;
    }
    else
    {
      fill_name = game::high_scores[i].initials;
      fill_score = game::high_scores[i].score;
    }

    sprintf (text_buffer, "%-14s %.7i", fill_name.c_str(), fill_score);
    text_image = TTF_RenderText_Solid (hsFont, text_buffer, text_color);
    dest.x = (settings::square_size * 20 - text_image->w) / 2;
    dest.y = 2 * (settings::square_size/2+settings::square_size/4) * i + settings::square_size * 4;
    dest.w = text_image->w;
    dest.h = text_image->h;
    
    SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  }

  for (i = 0; i < 20; ++i)
  {
    draw_block (shape_colors[rand() % 7], i,  0, 0);
    draw_block (shape_colors[rand() % 7], i, 22, 0);
  }

  SDL_Flip (game::screen);
}

void check_lines()
{
  int i, j, r, c;
  int sum;
  int last;
  int lines = 0;
  int mul[5] = { 0, 40, 100, 300, 1200 };
  
  uint32_t iclr;
  SDL_Color sclr;

  for (r = sum = last = 0; r < 20; ++r, sum = 0)
  {
    for (c = 0; c < 10; ++c)
      sum += game::board[c][r] ? 1 : 0;

    if (sum == 10)
    {
      ++lines;
      last = r;

      for (i = 0; i < 10; ++i)
      {
        sclr = blend (0x0, shape_colors[game::board[i][r] - 1]);
        iclr = (sclr.r << 16) | (sclr.g << 8) | (sclr.b);
        draw_block (iclr, i, r, 1);
      }
    }
  }

  game::score += mul[lines] * (game::level + 1);
  game::lines += lines;

  if (lines)
  {
    SDL_Flip (game::screen);

    for (i = 0; i < 10; ++i)
      for (j = last; j >= 0; --j)
        game::board[i][j] = (j - lines) < 0 ? 0 : game::board[i][j-lines];

    update_status_text();
  }

  if (game::lines >= 10)
    next_level();

  else
  {
    SDL_Delay (500);
    draw_board();
  }

  game::temp_tet = game::curr_tet;

  if (is_collision() )
  {
    game::state = STATE_SAVESCORE;
    game::score_save.initials = "---";
    game::score_save.n = 0;
    game::score_save.score = game::score;
    display_save_score();
  }
}

bool is_collision()
{
  int i, j;
  int x = game::temp_tet.x;
  int y = game::temp_tet.y;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (game::temp_tet.def[i][j] && (game::board[x+j][y+i] || ((x+j)<0) || ((x+j)>=10) || ((y+i)<0) || ((y+i)>=20)))
        return true;

  return false;
}

void draw_board()
{
  int i, j, x, y, clr;
  
  SDL_Rect dest;
  
  dest.x = settings::square_size;
  dest.y = settings::square_size;
  dest.w = 10 * settings::square_size;
  dest.h = 20 * settings::square_size;
  SDL_FillRect (game::screen, &dest, settings::background_color);

  for (i = 0; i < 10; ++i)
    for (j = 0; j < 20; ++j)
      if (game::board[i][j])
        draw_block (shape_colors[game::board[i][j] - 1], i, j, 1);

  x = game::curr_tet.x;
  y = game::curr_tet.y;
  clr = shape_colors[game::curr_tet.shape_idx - 1];

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (game::curr_tet.def[i][j])
        draw_block (clr, x + j, y + i, 1);

  dest.x = 14 * settings::square_size;
  dest.y = 16 * settings::square_size;
  dest.w =  4 * settings::square_size;
  dest.h =  4 * settings::square_size;
  SDL_FillRect (game::screen, &dest, settings::background_color);

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (game::next_tet.def[i][j])
        draw_block (shape_colors[game::next_tet.shape_idx-1], 13+j, 15+i, 1);

  SDL_Flip (game::screen);
}

void draw_frame (SDL_Surface *srfc, 
                 bool invert, 
                 uint32_t color, 
                 int x, int y, int w, int h)
{
  SDL_Color a;
  SDL_Color b;
  SDL_Color c;
  
  a = blend (invert ? 0x101010 : 0xf0f0f0, color);
  b = blend (invert ? 0xf0f0f0 : 0x101010, color);
  c = blend (0x808080, color);
  
  boxRGBA   (srfc,     x,     y, x+w-1,   y+1, a.r, a.g, a.b, 255);
  boxRGBA   (srfc,     x,     y,   x+1, y+h-1, a.r, a.g, a.b, 255);
  boxRGBA   (srfc,     x, y+h-2, x+w-1, y+h-1, b.r, b.g, b.b, 255);
  boxRGBA   (srfc, x+w-2,     y, x+w-1, y+h-1, b.r, b.g, b.b, 255);
  lineRGBA  (srfc,     x, y+h-1,   x+1, y+h-2, c.r, c.g, c.b, 255);
  lineRGBA  (srfc, x+w-1,     y, x+w-2,   y+1, c.r, c.g, c.b, 255);
  
  pixelRGBA (srfc,     x, y+h-2, a.r, a.g, a.b, 255);
  pixelRGBA (srfc, x+w-2,     y, a.r, a.g, a.b, 255);
}

void draw_status_frame (double y, double h)
{
  draw_frame (game::screen,
              false,
              settings::background_color,
              settings::square_size * 13,
              settings::square_size * (y + 1),
              settings::square_size * 6,
              settings::square_size * h);
}

void clear_status (int i)
{
  SDL_Rect r;
  
  r.x = 13 * settings::square_size + 2;
  r.y =  i * settings::square_size + 2;
  r.w =  6 * settings::square_size - 4;
  r.h =  2 * settings::square_size - 4;
  
  SDL_FillRect (game::screen, &r, settings::background_color);
}

void draw_status_text (int i, char *text_buffer, SDL_Color text_color)
{
  SDL_Rect dest;
  
  SDL_Surface *text_image = TTF_RenderText_Solid (game::text, text_buffer, text_color);
  
  dest.x = 13 * settings::square_size + (6 * settings::square_size - text_image->w) / 2;
  dest.y =  i * settings::square_size + (settings::square_size - text_image->h) / 2;
  dest.w = text_image->w;
  dest.h = text_image->h;
  
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
}

void update_status_text()
{
  char text_buffer[80];
  
  SDL_Color text_color =
  {
    (uint8_t)(settings::text_color >> 16),
    (uint8_t)(settings::text_color >>  8),
    (uint8_t)(settings::text_color >>  0),
    0
  };
  
  clear_status (1);
  clear_status (5);
  clear_status (8);
  
  sprintf (text_buffer, "SCORE");
  draw_status_text (1, text_buffer, text_color);
  
  sprintf (text_buffer, "%.7i", game::score);
  draw_status_text (2, text_buffer, text_color);
  
  sprintf (text_buffer, "LEVEL");
  draw_status_text (5, text_buffer, text_color);
  
  sprintf (text_buffer, "%i", game::level);
  draw_status_text (6, text_buffer, text_color);
  
  sprintf (text_buffer, "LINES");
  draw_status_text (8, text_buffer, text_color);
  
  sprintf (text_buffer, "%i", game::lines);
  draw_status_text (9, text_buffer, text_color);
}

void draw_block (uint32_t color, int col, int row, int offset)
{
  SDL_Rect dest =
  {
    (int16_t)(settings::square_size * (offset + col)),
    (int16_t)(settings::square_size * (offset + row)),
    (int16_t)settings::square_size,
    (int16_t)settings::square_size
  };
  
  SDL_FillRect (game::block, NULL, color);
  draw_frame (game::block, false, color, 0, 0, settings::square_size, settings::square_size);
  
  SDL_BlitSurface (game::block, NULL, game::screen, &dest);
}

SDL_Color blend (uint32_t a, uint32_t b)
{
  SDL_Color c =
  {
    (uint8_t)(((a >> 17) & 0x7f) + ((b >> 17) & 0x7f)),
    (uint8_t)(((a >>  9) & 0x7f) + ((b >>  9) & 0x7f)),
    (uint8_t)(((a >>  1) & 0x7f) + ((b >>  1) & 0x7f)),
    0
  };
  
  return c;
}

void init_tets()
{
  game::curr_tet.shape_idx = (rand() % 7) + 1;
  game::next_tet.shape_idx = (rand() % 7) + 1;
  game::curr_tet.rotate_idx = 0;
  game::next_tet.rotate_idx = 0;
  
  set_tet_array (game::curr_tet, 3, -1);
  set_tet_array (game::next_tet, 3, -1);
}

void cycle_tets()
{
  game::curr_tet = game::next_tet;
  game::next_tet.shape_idx = (rand() % 7) + 1;
  game::next_tet.rotate_idx = 0;
  
  set_tet_array (game::next_tet, 3, -1);
}

void set_tet_array (shape_t &tet, int x, int y)
{
  int i, j, shift;
  uint32_t def;
  
  tet.x = x;
  tet.y = y;
  def = shape_defs[tet.shape_idx - 1][tet.rotate_idx / 2];
  shift = (tet.rotate_idx & 1) ? 0 : 16;
  def = (def >> shift) & 0xffff;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      tet.def[i][j] = (def >> (15 - (4 * i + j) ) ) & 1;
}

void parse_menu (menu_e en)
{
  switch (en)
  {
  case MENU_YES:

    if (game::state != STATE_VERIFY)
      return;

    if (game::to_state == STATE_QUIT)
      exit (0);

    if (game::to_state == STATE_MENU)
    {
      game::state = STATE_MENU;
      display_menu();
    }

    break;
  case MENU_NO:

    if (game::state != STATE_VERIFY)
      return;

    if (game::from_state == STATE_MENU)
    {
      game::state = STATE_MENU;
      display_menu();
    }
    else if (game::from_state == STATE_PLAY)
    {
      game::state = STATE_PLAY;
      set_game_screen();
      draw_board();
      draw_commands (true);
    }
    else if (game::from_state == STATE_SCORES)
    {
      game::state = STATE_SCORES;
      display_scores();
      draw_commands (false);
    }
    else if (game::from_state == STATE_PAUSED)
    {
      game::state = STATE_PAUSED;
      set_game_screen();
      draw_board();
      display_pause();
    }

    break;
  case MENU_MENU:

    if (game::state == STATE_PLAY)
    {
      game::from_state = STATE_PLAY;
      game::state = STATE_VERIFY;
      game::to_state = STATE_MENU;
      verify_quit();
    }
    else if (game::state == STATE_SCORES)
    {
      game::state = STATE_MENU;
      display_menu();
    }

    break;
  case MENU_PAUSE:

    if (game::state != STATE_PLAY)
      return;

    game::state = STATE_PAUSED;
    display_pause();
    break;
  case MENU_NEWGAME:

    if (game::state != STATE_MENU)
      return;

    game::state = STATE_PLAY;

    for (int i = 0; i < 10; ++i)
      for (int j = 0; j < 20; ++j)
        game::board[i][j] = 0;

    init();
    set_game_screen();
    draw_board();
    draw_commands (true);
    break;
  case MENU_RESUME:

    if (game::state != STATE_PAUSED)
      return;

    game::state = STATE_PLAY;
    set_game_screen();
    draw_board();
    draw_commands (true);
    break;
  case MENU_SCORES:

    if (game::state != STATE_MENU)
      return;

    game::state = STATE_SCORES;
    display_scores();
    draw_commands (false);
    break;
  case MENU_QUIT:

    if (game::state == STATE_QUIT)
      break;

    game::from_state = game::state;
    game::state = STATE_VERIFY;
    game::to_state = STATE_QUIT;
    verify_quit();
    break;
  default:
    break;
  }
}

void move_shape (move_e en)
{
  if (game::state != STATE_PLAY)
    return;

  game::temp_tet = game::curr_tet;

  switch (en)
  {
  case MOVE_COUNTER:
    game::temp_tet.rotate_idx--;

    if (game::temp_tet.rotate_idx < 0)
      game::temp_tet.rotate_idx = 3;

    set_tet_array (game::temp_tet, game::temp_tet.x, game::temp_tet.y);

    if (!is_collision() )
    {
      game::curr_tet = game::temp_tet;
      draw_board();
    }

    break;
  case MOVE_CLOCK:
    ++game::temp_tet.rotate_idx;

    if (game::temp_tet.rotate_idx > 3)
      game::temp_tet.rotate_idx = 0;

    set_tet_array (game::temp_tet, game::temp_tet.x, game::temp_tet.y);

    if (!is_collision() )
    {
      game::curr_tet = game::temp_tet;
      draw_board();
    }

    break;
  case MOVE_LEFT:
    set_tet_array (game::temp_tet, game::temp_tet.x-1, game::temp_tet.y);

    if (!is_collision() )
    {
      game::curr_tet = game::temp_tet;
      draw_board();
    }

    break;
  case MOVE_RIGHT:
    set_tet_array (game::temp_tet, game::temp_tet.x+1, game::temp_tet.y);

    if (!is_collision() )
    {
      game::curr_tet = game::temp_tet;
      draw_board();
    }

    break;
  case MOVE_DOWN:
    set_tet_array (game::temp_tet, game::temp_tet.x, game::temp_tet.y+1);

    if (!is_collision() )
    {
      game::curr_tet = game::temp_tet;
      draw_board();
    }
    else
    {
      inscribe();
      cycle_tets();
      check_lines();
    }

    break;
  case MOVE_SLAM:

    while (true)
    {
      set_tet_array (game::temp_tet, game::temp_tet.x, game::temp_tet.y+1);

      if (!is_collision() )
      {
        game::curr_tet = game::temp_tet;
        draw_board();
      }
      else
      {
        inscribe();
        cycle_tets();
        check_lines();
        SDL_Delay (400);
        break;
      }
    }

    break;
  default:
    break;
  }
}

void inscribe()
{
  int i, j;
  int x = game::curr_tet.x;
  int y = game::curr_tet.y;

  for (i = 0; i < 4; ++i)
    for (j = 0; j < 4; ++j)
      if (game::curr_tet.def[i][j])
        game::board[x+j][y+i] = game::curr_tet.shape_idx;
}

void display_menu()
{
  int i;
  TTF_Font *name_font = TTF_OpenFont ("joystix.ttf", 
                                      (settings::square_size * 2) + 
                                      (settings::square_size / 2));
                                      
  TTF_Font *cmd_font = TTF_OpenFont ("joystix.ttf", settings::square_size);
  SDL_Rect dest;
  SDL_Surface *text_image;
  SDL_Color text_color = { 0, 255, 255, 0 };
  SDL_Color cmd_color = { 255, 0, 255, 0 };
  
  char name_buf[80];
  
  char text_buffer[8][80] =
  {
    { "NEW GAME"    },
    { "G"           },
    { "HIGH SCORES" },
    { "H"           },
    { "QUIT"        },
    { "Q"           }
  };
  
  SDL_FillRect (game::screen, NULL, settings::background_color);

  for (i = 0; i < 20; ++i)
  {
    draw_block (shape_colors[rand() % 7], i,  0, 0);
    draw_block (shape_colors[rand() % 7], i, 22, 0);
  }

  sprintf (name_buf, "blockstar");
  text_image = TTF_RenderText_Solid (name_font, name_buf, text_color);
  dest.x = (settings::square_size * 20 - text_image->w) / 2;
  dest.y = settings::square_size * 3;
  dest.w = text_image->w;
  dest.h = text_image->h;
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  
  dest.y = settings::square_size * 8;

  for (i = 0; i < 3; ++i)
  {
    text_image = TTF_RenderText_Solid (cmd_font, text_buffer[2*i], cmd_color);
    dest.y += settings::square_size * 2;
    dest.x = settings::square_size * 4;
    dest.w = text_image->w;
    dest.h = text_image->h;
    SDL_BlitSurface (text_image, NULL, game::screen, &dest);
    
    text_image = TTF_RenderText_Solid (cmd_font, text_buffer[2*i+1], cmd_color);
    dest.x = settings::square_size * 15;
    dest.w = text_image->w;
    dest.h = text_image->h;
    SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  }

  SDL_Flip (game::screen);
}

void display_level()
{
  char text_buffer[80];
  
  SDL_Rect dest, r;
  SDL_Surface *text_image;
  SDL_Color text_color =
  {
    text_color.r = (uint8_t)(settings::text_color >> 16),
    text_color.g = (uint8_t)(settings::text_color >>  8),
    text_color.b = (uint8_t)(settings::text_color >>  0),
    0
  };
  
  sprintf (text_buffer, "LEVEL %i", game::level);
  text_image = TTF_RenderText_Solid (game::text, text_buffer, text_color);
  
  dest.x = settings::square_size + (10 * settings::square_size - text_image->w) / 2;
  dest.y = settings::square_size + (20 * settings::square_size - text_image->h) / 2;
  dest.w = text_image->w;
  dest.h = text_image->h;
  
  r = dest;
  r.x -= settings::square_size;
  r.y -= settings::square_size;
  r.w += 2 * settings::square_size;
  r.h += 2 * settings::square_size;
  
  SDL_FillRect (game::screen, &r, settings::background_color);
  SDL_BlitSurface (text_image, NULL, game::screen, &dest);
  SDL_Flip (game::screen);
}

void next_level()
{
  int i, j;

  for (i = 0; i < 20; ++i)
    for (j = 0; j < 10; ++j)
      game::board[j][i] = 0;

  ++game::level;
  game::lines = 0;
  game::speed = (game::speed / 2) + (game::speed / 4);
  update_status_text();
  display_level();
  SDL_Delay (2000);
  init_tets();
  draw_board();
}

void set_game_screen()
{
  SDL_FillRect (game::screen, NULL, settings::background_color);
  
  draw_frame (game::screen,
              true,
              settings::background_color,
              settings::square_size - (settings::square_size / 4),
              settings::square_size - settings::square_size / 4,
              settings::square_size * 10 + settings::square_size / 2,
              settings::square_size * 20 + settings::square_size / 2);
  
  draw_status_frame (-0.1, 2.25);
  draw_status_frame ( 3.9, 2.25);
  draw_status_frame ( 6.9, 2.25);
  draw_status_frame (13.9, 6.25);
  draw_board();
  
  update_status_text();
}

void init()
{
  init_tets();
  game::score = 0;
  game::level = 0;
  game::lines = 0;
  game::speed = 800;
  
  set_game_screen();
}

void event_loop()
{
  SDL_Event event;
  int key_in;
  
  static int key_delay = SDL_GetTicks();
  static int fall_delay = key_delay;
  
  unsigned char *key_state;

  while (true)
  {
    if ((SDL_GetTicks() - fall_delay) >= game::speed)
    {
      fall_delay = SDL_GetTicks();
      key_state = SDL_GetKeyState (NULL);

      if (!key_state[SDLK_DOWN] && !key_state[SDLK_s])
        move_shape (MOVE_DOWN);
    }

    SDL_PollEvent (&event);

    switch (event.type)
    {
    case SDL_KEYDOWN:
      key_in = event.key.keysym.sym;

      if (SDL_GetTicks() - key_delay > 150)
        key_delay = SDL_GetTicks();
        
      else
        continue;

      if (game::state == STATE_SAVESCORE)
      {
        switch (key_in)
        {
        case SDLK_RETURN:
          save_to_file();
          game::state = STATE_SCORES;
          display_scores();
          draw_commands (false);
          break;
        case SDLK_BACKSPACE:

          if (!game::score_save.n)
            break;

          game::score_save.initials[--game::score_save.n] = '-';
          display_save_score();
          break;
        default:

          if (game::score_save.n == 3 || key_in < SDLK_a || key_in > SDLK_z)
            break;

          game::score_save.initials[game::score_save.n++] = (char) key_in;
          display_save_score();
          break;
        }
      }
      else
      {
        switch (key_in)
        {
        case SDLK_y:
          parse_menu (MENU_YES);
          break;
        case SDLK_n:
          parse_menu (MENU_NO);
          break;
        case SDLK_p:
          parse_menu (MENU_PAUSE);
          break;
        case SDLK_m:
          parse_menu (MENU_MENU);
          break;
        case SDLK_g:
          parse_menu (MENU_NEWGAME);
          break;
        case SDLK_h:
          parse_menu (MENU_SCORES);
          break;
        case SDLK_q:
          parse_menu (MENU_QUIT);
          break;
        case SDLK_r:
          parse_menu (MENU_RESUME);
          break;
        case SDLK_w:
          move_shape (MOVE_COUNTER);
          break;
        case SDLK_d:
          move_shape (MOVE_RIGHT);
          break;
        case SDLK_a:
          move_shape (MOVE_LEFT);
          break;
        case SDLK_s:
          move_shape (MOVE_DOWN);
          break;
        case SDLK_UP:
          move_shape (MOVE_COUNTER);
          break;
        case SDLK_DOWN:
          move_shape (MOVE_DOWN);
          break;
        case SDLK_LEFT:
          move_shape (MOVE_LEFT);
          break;
        case SDLK_RIGHT:
          move_shape (MOVE_RIGHT);
          break;
        default:
          break;
        }

        break;
        
      case SDL_QUIT:
        exit (0);
        
      default:
        break;
      }
    }
  }
}
