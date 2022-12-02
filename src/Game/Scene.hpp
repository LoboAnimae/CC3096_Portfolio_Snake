//
// Created by yagdrassyl on 1/12/22.
//

#ifndef SCENE_HPP_
#define SCENE_HPP_
#include <vector>
#include <string>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "Components.hpp"
#include <deque>

struct Entity;
class Setup;
class Input;
class Update;
class Render;

struct Color {
    int r;
    int g;
    int b;
    Uint8 a = 255;

    Color (int red, int green, int blue, int alpha = 255)
    {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }
};

struct Cell {
    int column;
    int row;
    Cell (int pRow, int pColumn) : row (pRow), column (pColumn)
    {};
};

class Scene {
 private:
  std::vector<Setup *> setupSystems;
  std::vector<Input *> inputSystems;
  std::vector<Update *> updateSystems;
  std::vector<Render *> renderSystems;

 public:
  explicit Scene (const std::string &);
  ~Scene () = default;

  entt::registry r;
  Entity *mainCamera;
  Entity *player;
  std::deque<Cell> occupiedCells;
  Entity *fruitCell;

  b2World *world;
  int gridSize;
  int gridCellSize;
  std::vector<std::vector<int>> grid;

  std::vector<Color> colors;

  int direction;
  int currentColorIndex;
  int changeEveryFrames;
  int currentFrame;
  Color getNextColor ();

  Uint32 collisionEvent;

  Entity createEntity (
      const std::string &name = "NO NAME",
      int x = 0,
      int y = 0
  );

  Entity createEntity (
      const std::string &name = "NO NAME",
      b2BodyType type = b2_staticBody,
      int x = 0,
      int y = 0
  );

  void addSetupSystem (Setup *s);
  void addInputSystem (Input *s);
  void addUpdateSystem (Update *s);
  void addRenderSystem (Render *s);

  void setup ();
  void update (double dT);
  void input (SDL_Event event);
  void render (SDL_Renderer *renderer);
};

#endif //SCENE_HPP_
