#include "common.h"
#include "Image.h"

#include "Player.h"
#include <string>
#include <fstream>
#include "unistd.h"


#define GLFW_DLL
#include <GLFW/glfw3.h>


  Image floor1("../resources/land.png");
  Image key("../resources/kaktus.png");
  Image hole("../resources/hole.png");
  Image wall("../resources/wall_2.png");
  Image final_door("../resources/door_final.png");
  Image open_door("../resources/opendoor.png");
  Image knight("../resources/knight.png");
  Image gameover("../resources/gameover.png");
  Image start_picture("../resources/start_picture.png");
  Image kaktus_solid("../resources/kaktus_color_line.png");
  Image kaktus_null("../resources/kaktus_null.png");
  Image wingame("../resources/win.png");


constexpr GLsizei WINDOW_WIDTH = 1300, WINDOW_HEIGHT = 1024;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(int* MAP, Player &player)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MAP, MovementDir::UP);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MAP, MovementDir::DOWN);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MAP, MovementDir::LEFT);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MAP, MovementDir::RIGHT); 
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void Rewrite_Mas_of_keys(int check_land_keys, int ( &Mas_of_keys)[4][5], int (&Location)[4][5])
{
  if (check_land_keys == 1)
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 5; j++)
        if (Location[i][j] == 1)
          Mas_of_keys[i][j] = 1;
}


int* DrawLevel(Image &screenBuffer, int Room_Map[], int (&Mas_of_keys)[4][5], int (&Location)[4][5], int side)
{
  int s, t;
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 5; j++)
      if(Location[i][j] == 1)
      {
        s = i;
        t = j;
        Location[i][j] = 0;
      }
  if (side == 1)
  {
      s--;
  }
  if (side == 2)
  {
      t--;
  }
  if (side == 3)
  {
      s++;
  }
  if (side == 4)
  {
      t++;
  }
  Location[s][t] = 1;// change room;
  int check_key = 0;
  if (Mas_of_keys[s][t] == 1)
    check_key = 1; // draw floor

  int *Map_mas_numb = new int[1024];
  if (Room_Map[s*5+t] == 1)
    Map_mas_numb = ReadMap("../lvl5.txt");
  if (Room_Map[s*5+t] == 2)
    Map_mas_numb = ReadMap("../lvl3.txt");
  if (Room_Map[s*5+t] == 3)
    Map_mas_numb = ReadMap("../lvl9.txt");
  if (Room_Map[s*5+t] == 4)
    Map_mas_numb = ReadMap("../lvl6.txt");
  if (Room_Map[s*5+t] == 5)
    Map_mas_numb = ReadMap("../lvl4.txt");
  if (Room_Map[s*5+t] == 6)
    Map_mas_numb = ReadMap("../lvl1.txt");
  if (Room_Map[s*5+t] == 7)
    Map_mas_numb = ReadMap("../lvl7.txt");
  if (Room_Map[s*5+t] == 8)
    Map_mas_numb = ReadMap("../lvl8.txt");
  if (Room_Map[s*5+t] == 9)
    Map_mas_numb = ReadMap("../lvl2.txt");
  if (Room_Map[s*5+t] == 10)
    Map_mas_numb = ReadMap("../lvl10.txt");

  for (int k = 0; k < 32; k++)//here we will read simbols from file
  {
    for(int s = 0; s < 32; s++)
    { 
        if(Map_mas_numb[k*32 + s] == -1)
          DrawTile(wall, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 0)
          DrawTile(floor1, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 1 && check_key == 0)
          DrawTile(key, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 1 && check_key == 1)
        {
          DrawTile(floor1, screenBuffer, s, k);
          Map_mas_numb[k*32 + s] == 0;
        }
        if(Map_mas_numb[k*32 + s] == 2)
          DrawTile(final_door, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 3)
          DrawTile(hole, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 4)
          DrawTile(open_door, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 5)
          DrawTile(floor1, screenBuffer, s, k); // player
    }
  }
  return Map_mas_numb;
}

void Draw_kaktus(Image &screenBuffer, int k)
{
  if(k == 1)
  {
  for (int i = 0; i < kaktus_null.Height(); i++)
    for (int j = 0; j < kaktus_null.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 400 + i, kaktus_null.GetPixel(j, i));
  }
  if(k == 2)
  {
  for (int i = 0; i < kaktus_null.Height(); i++)
    for (int j = 0; j < kaktus_null.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 600 + i, kaktus_null.GetPixel(j, i));
  }
  if(k == 3)
  {
  for (int i = 0; i < kaktus_null.Height(); i++)
    for (int j = 0; j < kaktus_null.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 800 + i, kaktus_null.GetPixel(j, i));
  }
}
void Draw_kaktus_sold(Image &screenBuffer, int k)
{
  if(k == 1)
  {
  for (int i = 0; i < kaktus_solid.Height(); i++)
    for (int j = 0; j < kaktus_solid.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 400 + i, kaktus_solid.GetPixel(j, i));
  }
  if(k == 2)
  {
  for (int i = 0; i < kaktus_null.Height(); i++)
    for (int j = 0; j < kaktus_null.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 600 + i, kaktus_solid.GetPixel(j, i));
  }
  if(k == 3)
  {
  for (int i = 0; i < kaktus_null.Height(); i++)
    for (int j = 0; j < kaktus_null.Width(); j++)
    screenBuffer.PutPixel(1024 + j, 800 + i, kaktus_solid.GetPixel(j, i));
  }
}
void GetDark(Image &screenBuffer, GLFWwindow* window)
{
  for (double i = 20; i > 1; i--)
  {
    double koef = i / 20;
    for (int i = 0; i < 1024; i++){
      for (int j = 0; j < 1024; j++){
        Pixel newpic;
        newpic.r = screenBuffer.GetPixel(j, i).r *(koef * 1);
        newpic.b = screenBuffer.GetPixel(j, i).b *(koef * 1);
        newpic.g = screenBuffer.GetPixel(j, i).g *(koef *  1);
        screenBuffer.PutPixel(j, i, newpic);
      }
    }
    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
  }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();



  int *Map_mas_numb = new int[1024];
  int *Room_Map = new int[20];
  Map_mas_numb = ReadMap("../lvl1.txt");
  Room_Map = ReadFirstFile("../map.txt");
 
  int Location[4][5];
  int Mas_of_keys[4][5];
  for (int i = 0; i < 4; i++)
  for(int j = 0; j < 5; j++)
  {
    Location[i][j] = 0;
    Mas_of_keys[i][j]=0;
  }
  Location[2][2]= 1; //starting pos

  int start_x, start_y;
  for(int i = 0; i < 1024; i++)
  {
      if(Map_mas_numb[i] == 5)
      {
        start_y = i / 32;
        start_x = i % 32;
        break;
      }
  }
	//Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
  Point starting_pos{.x = start_x*32, .y = start_y*32};
	Player player{starting_pos};

  
  Image stone("../resources/stone.png");
  Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  //DrawLevel(Map_mas_numb, screenBuffer);

  for (int k = 0; k < 32; k++)//here we will read simbols from file
  {
    for(int s = 0; s < 32; s++)
    { 
        if(Map_mas_numb[k*32 + s] == -1)
          DrawTile(wall, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 0)
          DrawTile(floor1, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 1)
          DrawTile(key, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 2)
          DrawTile(final_door, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 3)
          DrawTile(hole, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 4)
          DrawTile(open_door, screenBuffer, s, k);
        if(Map_mas_numb[k*32 + s] == 5)
          DrawTile(floor1, screenBuffer, s, k); // player
      //for (int i = 0; i < img.Height(); i++)
      //for (int j = 0; j < img.Width(); j++)
      //screenBuffer.PutPixel(k * 32 + j, s * 32 + i, img.GetPixel(j, i));
    }
  }
  //==================================
  for (int i = 0; i < start_picture.Height(); i++)
    for (int j = 0; j < start_picture.Width(); j++)
    screenBuffer.PutPixel(1055 + j, 80 + i, start_picture.GetPixel(j, i));
  
  
  
  Draw_kaktus(screenBuffer, 1);
  Draw_kaktus(screenBuffer, 2);
  Draw_kaktus(screenBuffer, 3);


  //screenBuffer.Save("../resources/fon.png");
  //Image bkground("../resources/fon.png");
  Image bkground(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  for(int y = 0; y <= 1024; ++y)
    {
      for(int x = 0; x <= 1024; ++x)
        bkground.PutPixel(x, y, screenBuffer.GetPixel(x, y));// ne nado
    }


  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

  int check_end = 0;
  //game loop
	while (!glfwWindowShouldClose(window))
	{
    player.flag_check = -1;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents();

    processPlayerMovement(Map_mas_numb, player);
    player.Draw(Map_mas_numb, hole, screenBuffer, bkground);

    if(player.flag_check == 1)//key
    {
      DrawTile(floor1, screenBuffer, player.check_coords.x / 32, player.check_coords.y / 32);
      if (player.check_coords.x % 32 >= 16)
      {
          DrawTile(floor1, screenBuffer, (player.check_coords.x)/ 32 + 1, 
          (player.check_coords.y)/ 32);
      }
      if (player.check_coords.x % 32 < 16)
      {
          DrawTile(floor1, screenBuffer, (player.check_coords.x)/ 32 - 1, 
          (player.check_coords.y)/ 32);
      }

      for(int y = 0; y <= 1024; ++y)
      {
        for(int x = 0; x <= 1024; ++x)
          bkground.PutPixel(x, y, screenBuffer.GetPixel(x, y));
      }

      Draw_kaktus_sold(screenBuffer, player.numb_of_keys);
    }
    if(player.flag_check == 2) //final door
    {
      if(player.numb_of_keys == 3)
      {
        for(int y = 0; y < 1024; ++y)
          for(int x = 0; x < 1300; ++x)
            screenBuffer.PutPixel(x, y, backgroundWhite);
      for (int i = 0; i < wingame.Height(); i++)
        for (int j = 0; j < wingame.Width(); j++)
          screenBuffer.PutPixel( j+200, i+200, wingame.GetPixel(j, i));
      check_end = 1;
      }
    }

    if(player.flag_check == 3)//hole
    {
      GetDark(screenBuffer, window);

      for(int y = 0; y < 1024; ++y)
        for(int x = 0; x < 1300; ++x)
          screenBuffer.PutPixel(x, y, backgroundColor);
      

      for (int i = 0; i < gameover.Height(); i++)
        for (int j = 0; j < gameover.Width(); j++)
          screenBuffer.PutPixel( j+200, i+200, gameover.GetPixel(j, i));
      check_end = 1;//end of game
    }

    if(player.flag_check == 4)//open door
    {
      GetDark(screenBuffer, window);
      
      Rewrite_Mas_of_keys(player.check_land_keys, Mas_of_keys, Location);
      
      Map_mas_numb =  DrawLevel(screenBuffer, Room_Map, Mas_of_keys, Location, player.side_of_door);

      player.check_land_keys = 0;
      for(int i = 0; i < 1024; i++)
      {
        if(Map_mas_numb[i] == 5)
        {
          start_y = i / 32;
          start_x = i % 32;
          break;
        }
      }
      starting_pos.x = start_x*32;
      starting_pos.y = start_y*32; 
	    player.SetStartingPos(starting_pos);
      
      for(int y = 0; y <= 1024; ++y)
      { 
        for(int x = 0; x <= 1024; ++x)
          bkground.PutPixel(x, y, screenBuffer.GetPixel(x, y));
      }

      //bkground = screenBuffer;
    }


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glRasterPos2f(-1, 1);
    glPixelZoom(1,-1);
    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    glfwSwapBuffers(window);
    if (check_end == 1)
    {
      sleep(3);
      break;
    }
		
	}

	glfwTerminate();
	return 0;
}
