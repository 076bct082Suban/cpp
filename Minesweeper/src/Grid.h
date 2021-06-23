#pragma once
#include "Game.h"
#include "TextureManager.h"
#include <SDL2/SDL.h>
#include <random>
#include <time.h>

#define EASY_DIFFICULTY 1
#define MEDIUM_DIFFICULTY 2
#define HARD_DIFFICULTY 3
#define HIDDEN 0
#define POPED 1
#define FLAGED 2

#define MINE 9

class Grid {
public:
  Grid(int width, int height);
  ~Grid();

  void init(int difficulty);
  void construct(int x, int y);
  void generateTexture();
  void pop(int i, int j);
  void flag(int i, int j);

  void render();
  void update();

  bool constructed();

  void handleMouseClick(SDL_Event event);

private:
  int **blockStates;
  int **blocks;
  int blockWidth;
  int widthCount;
  int heightCount;
  int bombCount;
  bool hasStarted = false;

  int gameWidth, gameHeight;

  int gameTime = 0;
  int flagCount;

  bool isConstructed;
  SDL_Texture *blockTexture[10];
  SDL_Texture *numberTexture[10];
  SDL_Texture *forwawrdSlashTexture;
  SDL_Texture *texture;
  SDL_Texture *flagTexture;
  SDL_Texture *scoreTexture;
  SDL_Texture *timeTexture;

  int gridStartPosX, gridStartPosY;
};
