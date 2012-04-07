#include <cstdint>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>


enum state_e
{
  STATE_MENU,
  STATE_PAUSED,
  STATE_PLAY,
  STATE_QUIT,
  STATE_SAVESCORE,
  STATE_SCORES,
  STATE_VERIFY
};

enum menu_e
{
  MENU_MENU,
  MENU_NEWGAME,
  MENU_NO,
  MENU_PAUSE,
  MENU_QUIT,
  MENU_RESUME,
  MENU_SCORES,
  MENU_YES
};

enum move_e
{
  MOVE_CLOCK,
  MOVE_COUNTER,
  MOVE_DOWN,
  MOVE_LEFT,
  MOVE_RIGHT,
  MOVE_SLAM,
};

struct score_ds
{
  int n;
  int score;
  std::string initials;
};

struct shape_t
{
  int def[4][4];
  int rotate_idx;
  int shape_idx;
  int x;
  int y;
};


SDL_Color blend(uint32_t a, uint32_t b);

void clear_status       (int i);
void draw_block         (uint32_t color, int row, int col, int offset);
void draw_commands      (bool show_pause);
void draw_frame         (SDL_Surface *srfc, 
                         bool invert, 
                         uint32_t color, 
                         int x, int y, int w, int h);
                         
void draw_status_frame  (double y, double h);
void draw_status_text   (int i, char *txt_buf, SDL_Color txt_clr);
void move_shape         (move_e en);
void parse_menu         (move_e en);
void set_tet_array      (shape_t &tet, int x, int y);

bool is_collision();
void check_lines();
void cycle_tets();
void display_level();
void display_menu();
void display_pause();
void display_save_score();
void display_scores();
void draw_board();
void event_loop();
void init();
void init_tets();
void inscribe();
void next_level();
void set_game_screen();
void update_status_text();
void verify_quit();

namespace game
{
  extern int board[10][20];
  extern int level;
  extern int lines;
  extern int score;
  extern int speed;
  
  extern SDL_Surface *block;
  extern SDL_Surface *screen;
  
  extern state_e from_state;
  extern state_e state;
  extern state_e to_state; 
   
  extern struct score_ds high_scores[10];
  extern struct score_ds score_save;
  extern struct score_ds temp_high;
  
  extern struct shape_t curr_tet;
  extern struct shape_t next_tet;
  extern struct shape_t temp_tet;
  extern TTF_Font *text;
}

const uint32_t shape_colors[7] =
{
  0xff0000,
  0x00ff00,
  0x000080,
  0x800080,
  0x00ffff,
  0xffff00,
  0xffa500
};

const uint32_t shape_defs[7][2]=
{
  { 0x00f02222, 0x00f02222 }, // I
  { 0x02260470, 0x03220071 }, // J
  { 0x06220074, 0x02230170 }, // L
  { 0x06600660, 0x06600660 }, // O
  { 0x03604620, 0x03604620 }, // S
  { 0x02700262, 0x00720232 }, // T
  { 0x06302640, 0x06302640 }  // Z
};




