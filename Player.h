#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"


void DrawTile(Image &Tile, Image &screenBuffer, int wi, int he);
int* ReadMap (const char *filename);
int* ReadFirstFile (const char *filename);
struct Point
{
  int x;
  int y;
};
struct NumbOfRoom
{
  int numb;

};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  void ProcessInput(int* Map_mas_numb, MovementDir dir);
  void Draw(int* Map_mas_numb, Image &knight, Image &screen, Image &bkground);
  int flag_check = -1;
  int numb_of_keys = 0;
  int check_land_keys = 0;
  int side_of_door = -1;
  int timer = 0;
  int move_side = 3;
  int last_move_side = 3;
  void SetStartingPos(Point pos){coords.x = pos.x; coords.y = pos.y;}
  Point check_coords {.x = 0, .y = 0};
private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 255, .b = 0, .a = 255};
  int move_speed = 8;

};

#endif //MAIN_PLAYER_H
