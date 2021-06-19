#include "Grid.h"

// easy = 10 x 8, 10, 80 x 80 block
// medium = 20 x 16 , 50, 40 x 40 block
// hard = 25 x 20 , 120 , 32 x 32 block

Grid::Grid(int width, int height) {
  gameHeight = height;
  gameWidth = width;
  isConstructed = false;
}

void Grid::init(int difficulty) {
  switch (difficulty) {
  case EASY_DIFFICULTY:
    blockWidth = 80;
    widthCount = 10;
    heightCount = 8;
    bombCount = 10;
    break;

  case MEDIUM_DIFFICULTY:
    blockWidth = 40;
    widthCount = 20;
    heightCount = 16;
    bombCount = 50;
    break;

  case HARD_DIFFICULTY:
    blockWidth = 32;
    widthCount = 25;
    heightCount = 20;
    bombCount = 120;
    break;

  default:
    break;
  }

  blocks = new int *[heightCount];
  for (int i = 0; i < heightCount; i++) {
    blocks[i] = new int[widthCount];
    for (int j = 0; j < widthCount; j++) {
      blocks[i][j] = 0;
    }
  }

  gridStartPosY = gameHeight / 2 - (blockWidth * heightCount / 2);
  gridStartPosX = gameWidth / 2 - (blockWidth * widthCount / 2);
}

void Grid::construct(int x, int y) {
  int count = bombCount;
  int i, j;
  std::srand(time(NULL));

  while (count != 0) {
    i = rand() % heightCount;
    j = rand() % widthCount;

    // Why THE FUCK DO i and x not point in the same direction
    if (j >= (x - 2) && j <= (x + 2) && i >= (y - 2) && i <= (y + 2)) {
      // When we are about to place a mine at/around starting position.
      continue;
    }

    if (blocks[i][j] == MINE) {
      continue;
    }
    blocks[i][j] = MINE;

    count--;
  }

  for (int i = 0; i < heightCount; i++) {
    for (int j = 0; j < widthCount; j++) {
      int bombCount = 0;

      if (blocks[i][j] == MINE)
        continue;

      // IDK hard code for everyone (edges and corners)?
      if (i == 0 && j == 0) {
        if (blocks[i + 1][j] == MINE)
          bombCount += 1;
        if (blocks[i][j + 1] == MINE)
          bombCount += 1;
        if (blocks[i + 1][j + 1] == MINE)
          bombCount += 1;

        blocks[i][j] = bombCount;
        continue;
      }
      if (i == heightCount - 1 && j == 0) {
        if (blocks[i - 1][j] == MINE)
          bombCount += 1;
        if (blocks[i][j + 1] == MINE)
          bombCount += 1;
        if (blocks[i - 1][j + 1] == MINE)
          bombCount += 1;

        blocks[i][j] = bombCount;
        continue;
      }
      if (i == 0 && j == widthCount - 1) {
        if (blocks[i + 1][j] == MINE)
          bombCount += 1;
        if (blocks[i][j - 1] == MINE)
          bombCount += 1;
        if (blocks[i + 1][j - 1] == MINE)
          bombCount += 1;

        blocks[i][j] = bombCount;
        continue;
      }
      if (i == heightCount - 1 && j == widthCount - 1) {
        if (blocks[i - 1][j] == MINE)
          bombCount += 1;
        if (blocks[i][j - 1] == MINE)
          bombCount += 1;
        if (blocks[i - 1][j - 1] == MINE)
          bombCount += 1;

        blocks[i][j] = bombCount;
        continue;
      }
      if (i == 0) {
        for (int k = i; k < i + 2; k++) {
          for (int l = j - 1; l < j + 2; l++) {

            if (blocks[k][l] == MINE)
              bombCount += 1;
          }
        }
        blocks[i][j] = bombCount;
        continue;
      }
      if (j == 0) {
        for (int k = i - 1; k < i + 2; k++) {
          for (int l = j; l < j + 2; l++) {

            if (blocks[k][l] == MINE)
              bombCount += 1;
          }
        }
        blocks[i][j] = bombCount;
        continue;
      }
      if (i == heightCount - 1) {
        for (int k = i - 1; k < i + 1; k++) {
          for (int l = j - 1; l < j + 2; l++) {

            if (blocks[k][l] == MINE)
              bombCount += 1;
          }
        }
        blocks[i][j] = bombCount;
        continue;
      }
      if (j == widthCount - 1) {
        for (int k = i - 1; k < i + 2; k++) {
          for (int l = j - 1; l < j + 1; l++) {

            if (blocks[k][l] == MINE)
              bombCount += 1;
          }
        }
        blocks[i][j] = bombCount;
        continue;
      }
      // Loopty Loop for simple ones
      for (int k = i - 1; k < i + 2; k++) {
        for (int l = j - 1; l < j + 2; l++) {

          if (blocks[k][l] == MINE)
            bombCount += 1;
        }
      }
      blocks[i][j] = bombCount;
    }
  }
  generateTexture();
  isConstructed = true;
}

void Grid::generateTexture() {
  for (int i = 0; i < 10; i++) {
    numTexture[i] = TextureManager::LoadGridValue(i);
  }
}

void Grid::update() {}
void Grid::render() {

  SDL_Rect tempRect;
  tempRect.w = blockWidth - 2;
  tempRect.h = blockWidth - 2;
  SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);
  for (int i = 0; i < heightCount; i++) {
    for (int j = 0; j < widthCount; j++) {
      tempRect.x = gridStartPosX + blockWidth * j;
      tempRect.y = gridStartPosY + blockWidth * i;
      SDL_RenderFillRect(Game::renderer, &tempRect);
      SDL_RenderCopy(Game::renderer, numTexture[blocks[i][j]], NULL, &tempRect);
    }
  }
}

void Grid::handleMouseClick(SDL_Event event) {
  int x = (event.button.x - gridStartPosX) / blockWidth;
  int y = (event.button.y - gridStartPosY) / blockWidth;

  std::cout << x << ", " << y << std::endl;

  if (event.button.button == SDL_BUTTON_LEFT) {
    if (constructed()) {
      // do the popping this.
    } else {
      // Construct
      construct(x, y);
    }
  }
}

bool Grid::constructed() { return isConstructed; }
Grid::~Grid() {
  for (int i = 0; i < heightCount; i++) {
    delete[] blocks[i];
  }
}
