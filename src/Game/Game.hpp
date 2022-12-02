//
// Created by yagdrassyl on 1/12/22.
//

#ifndef _GAME_HPP_
#define _GAME_HPP_
#include <SDL2/SDL.h>
#include "Scene.hpp"
class Game {
  bool isRunning = false;
  SDL_Window *window;
  SDL_Renderer *renderer;
  int screenWidth;
  int screenHeight;
  int FPS;
  Uint32 frameStartTimestamp;
  Uint32 frameEndTimestamp;
  double frameDuration;
  float deltaTime;

 public:
  void init (const char *title, int width, int height);
  void setup ();
  void frameStart ();
  void frameEnd ();
  void handleEvents ();
  void update ();
  void render ();
  void clean ();
  bool running ();

  Game();
  ~Game();
};

#endif //_GAME_HPP_
