//
// Created by yagdrassyl on 1/12/22.
//

#include "Game.hpp"
#include "SystemImpl.hpp"
void Game::init (const char *title, int width, int height)
{
  if (SDL_Init (SDL_INIT_EVERYTHING) == 0)
    {
      window = SDL_CreateWindow (title, 0, 0, width, height, 0);
      renderer = SDL_CreateRenderer (window, -1, 0);
      screenWidth = width;
      screenHeight = height;

      isRunning = true;
    }
}
Scene *scene;
void Game::setup ()
{
  scene = new Scene ("Scene 1");
  scene->addSetupSystem (new WorldSetup (screenWidth, 12));
  scene->addSetupSystem (new BackgroundDecorationSetup ());
  scene->addSetupSystem (new CameraSetup (1, screenWidth, screenWidth, screenWidth, screenWidth));
  scene->addSetupSystem (new CharacterSetup ());
  scene->addInputSystem (new PlayerInput ());
  scene->addUpdateSystem (new MovementUpdate ());
  scene->addUpdateSystem (new PlayerCollisionUpdate ());
  scene->addUpdateSystem (new FruitUpdater ());
  scene->addRenderSystem (new BackgroundDecorationRender ());
  scene->addRenderSystem (new PlayerRender ());
  scene->addRenderSystem (new PlayerBodyRender ());
  scene->addRenderSystem (new FruitRender ());
  scene->setup ();

}
void Game::frameStart ()
{
  frameStartTimestamp = SDL_GetTicks ();
  if (frameEndTimestamp)
    {
      deltaTime = (float) (frameStartTimestamp - frameEndTimestamp) / 1000.0f;
    }
  else
    {
      deltaTime = 0;
    }
}
void Game::frameEnd ()
{
  frameEndTimestamp = SDL_GetTicks ();
  float actualFrameDuration = frameEndTimestamp - frameStartTimestamp;
  if (actualFrameDuration < frameDuration)
    {
      SDL_Delay (frameDuration - actualFrameDuration);
    }
}

void Game::handleEvents ()
{
  SDL_Event event;
  while (SDL_PollEvent (&event) != 0)
    {
      if (event.type == SDL_QUIT)
        {
          isRunning = false;
        }

      scene->input (event);
    }
}
void Game::update ()
{
//  auto playerMovement = scene->player->getComponent<Components::Movement> ();
//  std::cout << playerMovement.vx << ", " << playerMovement.vy << "\n";
  scene->update (deltaTime);
}
void Game::render ()
{
  SDL_SetRenderDrawColor (renderer, 0, 0, 0, 1);
  SDL_RenderClear (renderer);
  scene->render (renderer);
  SDL_RenderPresent (renderer);
}
void Game::clean ()
{
  SDL_DestroyWindow (window);
  SDL_DestroyRenderer (renderer);
  SDL_Quit ();
}
bool Game::running ()
{
  return isRunning;
}

Game::Game ()
{
  FPS = 60;
  frameDuration = (1.0f / (float) FPS) * 1000.0f;
  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

Game::~Game () = default;