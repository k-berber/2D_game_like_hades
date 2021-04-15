#include "Player.h"

Image sonic_r1("../resources/sonic_r1.png");
Image sonic_r2("../resources/sonic_r2.png");
Image sonic_r3("../resources/sonic_r3.png");
Image sonic_l1("../resources/sonic_l1.png");
Image sonic_l2("../resources/sonic_l2.png");
Image sonic_l3("../resources/sonic_l3.png");
Image sonic_u1("../resources/sonic_u1.png");
Image sonic_u2("../resources/sonic_u2.png");
Image sonic_u3("../resources/sonic_u3.png");
Image sonic_d1("../resources/sonic_d1.png");
Image sonic_d2("../resources/sonic_d2.png");
Image sonic_d3("../resources/sonic_d3.png");


void DrawTile(Image &Tile, Image &screenBuffer, int wi, int he)
{
  //Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  for (int i = 0; i < Tile.Height(); i++)
    for (int j = 0; j < Tile.Width(); j++)
    screenBuffer.PutPixel(wi * 32 + j,  he * 32 + i, Tile.GetPixel(j, i));

}
int* ReadMap (const char *filename)
{
  FILE *fp;
  int* Mas = new int[1024];
  char read_ar[1100];
  if((fp = fopen(filename, "rb")) == NULL)
  {
    printf("someting wrong. cannot open this file");
  } 
  fread(read_ar, sizeof(char), 1100, fp);
  fclose(fp);
  int j = 0;
  int t = 0;
  for (int i = 0; i < 1100; i++)
  {

    if(read_ar[i] =='\n')
      t++;
    if(read_ar[i] == '#'){  
      Mas[j] = -1;
      j++;}
    if(read_ar[i] == '.'){  
      Mas[j] = 0;
      j++;}
    if(read_ar[i] == '@'){  
      Mas[j] = 5;
      j++;}
    if (read_ar[i] == 'K'){  
      Mas[j] = 1;
      j++;}
    if (read_ar[i] == 'Q'){  
      Mas[j] = 2;
      j++;}
    if (read_ar[i] == ' '){  
      Mas[j] = 3;
      j++;}
    if (read_ar[i] == 'x'){  
      Mas[j] = 4;
      j++;}  
  }
  //printf("%i, %i",j, t);
  return Mas;
}

int* ReadFirstFile (const char *filename)
{
  FILE *fp;
  int* Room_Map = new int[20];
  char read_ar[30];
  if((fp = fopen(filename, "rb")) == NULL)
  {
    printf("someting wrong. cannot open this file");
  } 
  fread(read_ar, sizeof(char), 30, fp);
  fclose(fp);
  int j = 0;
  int t = 0;
  for (int i = 0; i < 30; i++)
  {

    if(read_ar[i] =='\n')
      t++;
    if(read_ar[i] == 'A'){  
      Room_Map[j] = 1;
      j++;}
    if(read_ar[i] == 'B'){  
      Room_Map[j] = 2;
      j++;}
    if(read_ar[i] == 'C'){  
      Room_Map[j] = 3;
      j++;}
    if (read_ar[i] == 'D'){  
      Room_Map[j] = 4;
      j++;}
    if (read_ar[i] == 'E'){  
      Room_Map[j] = 5;
      j++;}
    if (read_ar[i] == 'F'){  
      Room_Map[j] = 6;
      j++;}
    if (read_ar[i] == 'G'){  
      Room_Map[j] = 7;
      j++;}
    if (read_ar[i] == 'H'){  
      Room_Map[j] = 8;
      j++;}
    if (read_ar[i] == 'I'){  
      Room_Map[j] = 9;
      j++;} 
    if (read_ar[i] == 'J'){  
      Room_Map[j] = 10;
      j++;} 
  }
  return Room_Map;
}

static Pixel blend(Pixel oldPixel, Pixel newPixel)
{
  newPixel.r = newPixel.a / 255.0 * (newPixel.r - oldPixel.r) + oldPixel.r;
  newPixel.g = newPixel.a / 255.0 * (newPixel.g - oldPixel.g) + oldPixel.g;
  newPixel.b = newPixel.a / 255.0 * (newPixel.b - oldPixel.b) + oldPixel.b;
  newPixel.a = 255;
  return newPixel;
}





bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(int* Map_mas_numb, MovementDir dir)
{
  int move_dist = move_speed * 1;
  timer++;
  switch(dir)
  {
    int height, width, height2, width2;
    case MovementDir::DOWN://inverse up and down
      move_side = 3; 
      height = coords.x / 32;
      height2 = (coords.x + 25) / 32; //+31
      width = (coords.y + 32 + move_dist-2) / 32;
      
      if (Map_mas_numb[height + 32 * width] != -1 && Map_mas_numb[height2 + 32 * width] != -1)  
      {
        old_coords.y = coords.y;
        coords.y += move_dist;//here we can check blocks
        break;
      }
      break;
    case MovementDir::UP:
      move_side = 1;
      height = coords.x / 32;
      height2 = (coords.x + 25) / 32; // +31
      width = (coords.y - move_dist) / 32;
      if (Map_mas_numb[height + width * 32] != -1 && Map_mas_numb[height2 + width * 32] != -1)
      //if (Map_mas_numb[height + width * 32] != -1)
      {
        old_coords.y = coords.y;
        coords.y -= move_dist;
        break;
      }
      break;
    case MovementDir::LEFT:
      move_side = 2;
      height =  (coords.x - move_dist+0) / 32;
      width = (coords.y) / 32;
      width2 = (coords.y+31) / 32;
      if (Map_mas_numb[height + width * 32] != -1 && Map_mas_numb[height + width2 * 32] != -1)
      {
        old_coords.x = coords.x;
        coords.x -= move_dist;
        break;
      }
      break;
    case MovementDir::RIGHT:
      move_side = 4;
      height =  (coords.x + 25 + move_dist-0) / 32; //+32
      width = (coords.y) / 32;
      width2 = (coords.y+31) / 32;
      if (Map_mas_numb[height + width * 32] != -1 && Map_mas_numb[height + width2 * 32] != -1)
      {
        old_coords.x = coords.x;
        coords.x += move_dist;
        break;
      }
      break;
    default:
      break;
  }
}

void Player::Draw(int* Map_mas_numb, Image &knight, Image &screen, Image &bkground)
{
  if(Moved())
  {
    for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y)
    {
      for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x)
      {
        screen.PutPixel(x, y, bkground.GetPixel(x, y));// ne nado
      }
    }
    //old_coords = coords;
    old_coords = coords;
  }
  
  
  if (timer >= 10 || last_move_side != move_side)
    timer = 0;
  if (move_side == 1)// && (timer <= 2 || 5 < timer <= 7))
  {
    if((timer <= 2 || 5 < timer <= 7))
    {
    for (int i = 0; i < sonic_r1.Height(); i++)
      for (int j = 0; j < sonic_r1.Width(); j++)
      {
        screen.PutPixel(coords.x + j,  coords.y + i, 
        blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_u1.GetPixel(j, i))); //hero
      }
    }
    if ((2 < timer <= 5))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
      {
        screen.PutPixel(coords.x + j,  coords.y + i, 
        blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_u2.GetPixel(j, i))); //hero
      }
    }
    if(7 < timer)
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
    for (int j = 0; j < sonic_r1.Width(); j++)
    {
      screen.PutPixel(coords.x + j,  coords.y + i, 
      blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_u3.GetPixel(j, i))); //hero
    }
    }
  }
  
  else if (move_side == 4)// && (timer <= 2 || 5 < timer <= 7))
  {
    if(timer <= 2 || 5 < timer <= 7)
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          //screen.PutPixel(coords.x + j,  coords.y + i, sonic_r1.GetPixel(j, i)); //hero
          screen.PutPixel(coords.x + j,  coords.y + i, 
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_r1.GetPixel(j, i))); //hero
        }
    }
    if((2 < timer <= 5))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i, 
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_r2.GetPixel(j, i))); //hero
        }
    }
    if(7 < timer)
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i, 
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_r3.GetPixel(j, i))); //hero
        }
    }
  }
  
  else if (move_side == 2)// && (timer <= 2 || 5 < timer <= 7))
  {
    if((timer <= 2 || 5 < timer <= 7))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
        //screen.PutPixel(coords.x + j,  coords.y + i, sonic_r1.GetPixel(j, i)); //hero
          screen.PutPixel(coords.x + j,  coords.y + i, 
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_l1.GetPixel(j, i))); //hero

          //Pixel pix = blend(screen.data[y * screen.Width() + x], sonic_r1.GetPixel(x - coords.x, ))
        }
    }
    if((2 < timer <= 5))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i, 
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_l2.GetPixel(j, i))); //hero
        }
    }
    if ( 7 < timer)
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
    for (int j = 0; j < sonic_r1.Width(); j++)
    {
      screen.PutPixel(coords.x + j,  coords.y + i, 
      blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_l3.GetPixel(j, i))); //hero
    }
    }
  }
  else if (move_side == 3)// && (timer <= 2 || 5 < timer <= 7))
  {
    if((timer <= 2 || 5 < timer <= 7))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i,
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_d1.GetPixel(j, i))); //hero
        }
    }
    if((2 < timer <= 5))
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i,
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_d2.GetPixel(j, i))); //hero
        }
    }
    if(7 < timer)
    {
      for (int i = 0; i < sonic_r1.Height(); i++)
        for (int j = 0; j < sonic_r1.Width(); j++)
        {
          screen.PutPixel(coords.x + j,  coords.y + i,
          blend(bkground.GetPixel(coords.x + j, coords.y + i), sonic_d3.GetPixel(j, i))); //hero
        }
    }
  }
  last_move_side = move_side;

  //------------------------------------------------------------------------
  int height = (old_coords.x+16) / 32;
  int width = (old_coords.y+16) / 32;
  if( Map_mas_numb[height + width * 32] == 1 && check_land_keys == 0)
  {
    //key check
    flag_check = 1;
    check_land_keys = 1;
    numb_of_keys++;
    //printf("numb %i", numb_of_keys);
    check_coords.x = old_coords.x + 16;
    check_coords.y = old_coords.y + 16;
  }
  else if( Map_mas_numb[height + width * 32] == 2)
  {
    flag_check = 2;
    // final door
  }
  else if( Map_mas_numb[height + width * 32] == 3)
  {
    flag_check = 3;
    //hole
  }
  else if( Map_mas_numb[height + width * 32] == 4)
  {
    //open door
    //
    if (coords.x > 900)//down
    {
      side_of_door = 4;
      //printf("right");
    }
    if (coords.y > 900)//right
    {
      side_of_door = 3;
      //printf("down");
    }
    if (coords.y < 100 )//left this up
    {
      //printf("up");
      side_of_door = 1;
    }
    if (coords.x <100)//up
    {
      //printf("left");
      side_of_door = 2;
    }
    flag_check = 4;
  }
  //}

  //DrawTile(knight, screen, coords.x, coords.x);



}