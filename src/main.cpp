#include <iostream>
#include "./Game/Game.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main ()
{
  auto *game = new Game ();

  game->init ("Snake Because F Me?", SCREEN_WIDTH, SCREEN_WIDTH);
  game->setup ();

  while (game->running ())
    {
      game->frameStart ();
      game->handleEvents ();
      game->update ();
      game->render ();
      game->frameEnd ();
    }

  game->clean ();

  return 0;

}
