#include <iostream>
#include <chrono>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

int const x = 50;
int const y = 20;

long frames = 0;

int snake_x, snake_y, score;

bool gameOver;

std::string board_header = "";
std::string board_body = "";
std::string board = "";
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

int coords[2] = {2, 3};

void setUp()
{
  gameOver = false;
  dir = STOP;
  snake_x = x/2;
  snake_y = y/2;
  score = 0;
}

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void init_board(int x, int y)
{
  for (int i = 0; i < x + 2; i++)
  {
    if ((i == x + 1) || (i % (x + 2) == 0))
    {
      board_header.append("|");
      board_body.append("|");

      if (i == x + 1) {
        board_header.append("\n");
        board_body.append("\n");
      }
    }
    else
    {
      board_header.append("-");
      board_body.append(" ");
    }
  }

  for (int i = 0; i < y + 2; i++)
  {
    if ((i == 0) || (i == y + 1))
    {
      board.append(board_header);
    }
    else
    {
      board.append(board_body);
    }
  }
}

void spawnFruit()
{
  coords[0] = rand() % x + 1;
  coords[1] = rand() % y + 1;
}

void renderFruit() {
  gotoxy(coords[0], coords[1]);
  std::cout << "F";
}

void render()
{
  gotoxy(0, 0);
  std::cout << board;
  renderFruit();
  gotoxy(snake_x, snake_y);
  std::cout << 'O';
  gotoxy(x/2 - 3, y + 3);
  std::cout << coords[0] << " - " << coords[1] << "\n" << "Score: " << score;
}

void input()
{
  if (_kbhit()) 
  {
    switch (_getch())
    {
    case 'w':
      dir = UP;
      break;
    case 's':
      dir = DOWN;
      break;
    case 'a':
      dir = LEFT;
      break;
    case 'd':
      dir = RIGHT;
      break;
    case 'x':
      gameOver = true;
    default:
      break;
    }
  }
}

void logic()
{
  switch (dir)
  {
  case 1:
    snake_x -= 1;
    if (snake_x == 0) {
      snake_x = x;
    }
    break;
  case 2:
    snake_x += 1;
    if (snake_x == x) {
      snake_x = 0;
    }
    break;
  case 3:
    snake_y -= 1;
    if (snake_y == 0) {
      snake_y = y + 1;
    }
    break;
  case 4:
    snake_y += 1;
    if (snake_y == y + 1) {
      snake_y = 0;
    }
    break;
  default:
    break;
  }

  if (snake_x == coords[0] && snake_y == coords[1]) {
    score += 1;
    spawnFruit();
  }
}

int main()
{
  setUp();

  init_board(x, y);

  system("cls");

  while (!gameOver)
  {
    render();
    input();
    logic();

    if ((dir == 3) || (dir == 4)) {
      Sleep(1000/6);
    } else {
      Sleep(1000/10);
    }

    frames += 1;
  };
}