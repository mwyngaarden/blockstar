#include <cstdint>
#include <fstream>
#include <string>

#include "blockstar.hpp"

using namespace std;

namespace settings
{
  char window_title[256] = "BLOCKSTAR";
  
  int frames_per_second = 100;
  int square_size = 24;
  int text_size = 10;
  int window_bpp = 32;
  int window_h = 24 * 20; // square_size * 20
  int window_w = 24 * 23; // square_size * 23
  
  uint32_t background_color = 0x0;
  uint32_t text_color = 0x808080;
}

string get_token (const string &str, int n, string delim)
{
  string ret_str;

  for (size_t i = 0, t = 1; i < str.size() && t <= (n + 1); ++i)
  {
    if (!str.compare (i, 1, delim) )
      ++t;
    else if (t == (n + 1))
      ret_str.append (str, i, 1);
  }

  return ret_str;
}

string remove_ws (string str)
{
  size_t found;
  
  found = str.find(" ");
  
  while (found != string::npos)
  {
    str.erase (found, 1);
    found = str.find (" ");
  }
  
  return str;
}

void get_settings()
{
  string str, a, b;
  
  ifstream file_config;
  file_config.open ("blockstar.cfg", ios::in);

  // TODO: need error handling!
  if (!file_config.is_open())
    return;

  while (getline (file_config, str) )
  {
    if (str.empty() )
      continue;

    str = remove_ws (str);

    if (str.substr (0, 1) == "#")
      continue;

    a = get_token (str, 1, "=");
    b = get_token (str, 2, "=");

    if (a == "square_size")
      settings::square_size = atoi (b.c_str() );
      
    else if (a == "text_size")
      settings::text_size = atoi (b.c_str() );
      
    else if (a == "frames_per_second")
      settings::frames_per_second = atoi (b.c_str() );
  }
}

void save_to_file ()
{
  ofstream file;
  file.open ("scores.dat", ios::out | ios::app);
  file << game::score_save.initials << ":" << game::score_save.score << endl;
  file.close();
}
