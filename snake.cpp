#include <iostream>
#include <chrono>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

int const x = 50;
int const y = 20;

int snake_x, snake_y, prev_x, prev_y, score, prev_score, frames, tail[100], tailPosition[100][2], coords[2], prevDir;

std::string board_header, board_body, board;

bool gameOver;

enum eDirection
{
  STOP = 0,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

eDirection dir;

void spawnFruit()
{
  coords[0] = rand() % x + 1;
  coords[1] = rand() % y + 1;
}

void setUp()
{
  board_header = "";
  board_body = "";
  board = "";
  gameOver = false;
  dir = STOP;
  snake_x = x / 2;
  snake_y = y / 2;
  score = 0;
  prev_score = 0;
  frames = 0;
  dir = DOWN;
  spawnFruit();
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
  int grass;

  for (int i = 0; i < x + 2; i++)
  {
    grass = rand() % 100;

    if ((i == x + 1) || (i % (x + 2) == 0))
    {
      board_header.append(" ");
      board_body.append(i % 5 == 0 ? "^" : " ");

      if (i == x + 1)
      {
        board_header.append("\n");
        board_body.append("\n");
      }
    }
    else
    {
      board_header.append(i % 3 == 0 ? "`" : " ");
      board_body.append(grass > 70 ? (i % 2 == 0 ? "^" : "`") : " ");
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
      if (i % 2 == 0)
      {
        board.append(board_body);
      }
      else
      {
        board.append(" ");
        board.append(board_body);
        board.erase((i * (x + 3)) + x + 2, 1);
      }
    }
  }
}

void renderFruit()
{
  gotoxy(coords[0], coords[1]);
  std::cout << "\033[41m"
            << "\033[93m"
            << "*"
            << "\033[0m"
            << "\033[0m";
}

void render()
{
  gotoxy(0, 0);
  std::cout << "\033[47m"
            << "\033[32m" << board << "\033[0m"
            << "\033[0m";
  renderFruit();
  gotoxy(snake_x, snake_y);

  if (dir > 2)
  {
    gotoxy(snake_x, snake_y);
    std::cout << "\033[42m"
              << "\033[91m"
              << ":"
              << "\033[0m"
              << "\033[0m";
  }
  else
  {
    gotoxy(snake_x, snake_y);
    std::cout << "\033[42m"
              << "\033[91m"
              << ":"
              << "\033[0m"
              << "\033[0m";
  }

  for (int i = 0; i < score; i++)
  {
    gotoxy(tailPosition[i][0], tailPosition[i][1]);

    if ((i > 0) && (i == score - 1))
    {
      std::cout << "\033[42m"
                << "\033[31m" << (dir == 1 ? ">" : "") << (dir == 2 ? "<" : "") << (dir == 3 ? "V" : "") << (dir == 4 ? "A" : "") << "\033[0m"
                << "\033[0m";
    }
    else
    {
      std::cout << "\033[42m"
                << "\033[33m" << (dir < 3 ? "-" : "|") << "\033[0m"
                << "\033[0m";
    }
  }

  gotoxy(x / 2 - 7, y + 3);
  std::cout << "       .-----.\n";
  gotoxy(x / 2 - 7, y + 4);
  std::cout << "Score: | "
            << "\033[32m" << (score < 10 ? "00" : "") << ((score >= 10) && (score < 100) ? "0" : "") << score << "\033[0m"
            << " |\n";
  gotoxy(x / 2 - 7, y + 5);
  std::cout << "       '-----'\n";
}

void input()
{
  prevDir = dir;

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
  prev_x = snake_x;
  prev_y = snake_y;

  switch (dir)
  {
  case 1:
    snake_x -= 1;
    if (snake_x == -1)
    {
      snake_x = x + 1;
    }

    if (prevDir == 2)
    {
      snake_y += 1;
    }
    break;
  case 2:
    snake_x += 1;
    if (snake_x == x + 2)
    {
      snake_x = -1;
    }

    if (prevDir == 1)
    {
      snake_y -= 1;
    }
    break;
  case 3:
    snake_y -= 1;
    if (snake_y == -1)
    {
      snake_y = y + 1;
    }

    if (prevDir == 4)
    {
      snake_x += 1;
    }
    break;
  case 4:
    snake_y += 1;
    if (snake_y == y + 1)
    {
      snake_y = -1;
    }

    if (prevDir == 3)
    {
      snake_x -= 1;
    }
    break;
  default:
    break;
  }

  if (score == 100)
  {
    gameOver = true;
  }

  if (snake_x == coords[0] && snake_y == coords[1])
  {
    score += 1;
    spawnFruit();
  }

  if (score > prev_score)
  {
    tail[score - 1] = 1;
    prev_score += 1;
  }

  int prev_tail[2] = {prev_x, prev_y};
  int prev_prev_tail[2];

  for (int i = 0; i < score; i++)
  {

    if (snake_x == tailPosition[i][0] && snake_y == tailPosition[i][1])
    {
      gameOver = true;
    }

    if (i == 0)
    {
      prev_tail[0] = tailPosition[0][0];
      prev_tail[1] = tailPosition[0][1];
      tailPosition[0][0] = prev_x;
      tailPosition[0][1] = prev_y;
    }
    else
    {
      prev_prev_tail[0] = tailPosition[i][0];
      prev_prev_tail[1] = tailPosition[i][1];
      tailPosition[i][0] = prev_tail[0];
      tailPosition[i][1] = prev_tail[1];
      prev_tail[0] = prev_prev_tail[0];
      prev_tail[1] = prev_prev_tail[1];
    }
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

    if ((dir == 3) || (dir == 4))
    {
      Sleep(1000 / 5);
    }
    else
    {
      Sleep(1000 / 10);
    }

    frames += 1;
  };

  gotoxy(0, 0);

  for (int i = 0; i < y + 2; i++)
  {
    for (int j = 0; j < x + 2; j++)
    {
      if ((i >= 4 && i < y - 2) && (j >= 4 && j < x - 2))
      {
        std::cout << " ";
      }
      else
      {
        std::cout << "-";
      }
    }
    std::cout << "\n";
  }

  gotoxy(12, 8);

  std::cout << "\033[32m" << " __  __  __   __  __";
  gotoxy(12, 9);

  std::cout << "|_  |   |  | | / |__ *    " << "\033[92m" << score;
  gotoxy(12, 10);

  std::cout << "\033[32m" << "__| |__ |__| | \\ |__ *" << "\033[0m\n";

  gotoxy((x/2) - 5, (y/2) + 3);

  std::cout << "GAME OVER!";

  gotoxy(x / 2 - 7, y + 3);

  std::cout << "                                       \n                                          \n                                          \n";

  Sleep(10000);
  system("cls");
}