#include <cstdint>
#include <string>

namespace settings
{
  extern char window_title[256];
  
  extern int frames_per_second;
  extern int square_size;
  extern int text_size;
  extern int window_bpp;
  extern int window_h;
  extern int window_w;
  
  extern uint32_t background_color;
  extern uint32_t text_color;
}

std::string get_token   (const std::string &str, int n, std::string delim);
std::string remove_ws   (std::string str);

void get_settings       ();
void save_to_file       ();

