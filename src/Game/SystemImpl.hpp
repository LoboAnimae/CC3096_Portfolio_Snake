//
// Created by yagdrassyl on 1/12/22.
//

#ifndef SYSTEMIMPL_HPP_
#define SYSTEMIMPL_HPP_

#include "Systems.hpp"
#include "Components.hpp"
#include "Entities.hpp"
#include <iostream>
#include <random>
Cell getRandomPosition (Scene *scene)
{
  std::random_device rd;
  std::mt19937 mt (rd ());
  auto max = scene->gridSize;
  std::uniform_real_distribution<double> dist (0.f, max);
  do
    {
      Cell position = Cell (dist (mt), dist (mt));

      for (auto occupied: scene->occupiedCells)
        {
          if (occupied.column == position.column && occupied.row == position.row) continue;
        }
      return position;
    }
  while (true);
}

struct WorldSetup : public Setup {
  WorldSetup (int pScreenWidth, int pGridSize)
  {
    screenWidth = pScreenWidth;
    gridSize = pGridSize;
  }

  void run () override
  {
    scene->gridSize = gridSize;
    auto gridCellSize = screenWidth / gridSize;
    scene->gridCellSize = gridCellSize;

    std::cout << "Set gridSize to " << gridSize << " and gridCellSize to " << scene->gridCellSize << "\n";
    std::vector<std::vector<int>> rows;

    for (int i = 0; i < gridSize; ++i)
      {
        std::vector<int> columns;
        for (int j = 0; j < gridSize; ++j)
          {
            columns.push_back (0);
          }
        rows.push_back (columns);
      }
    scene->occupiedCells.emplace_back (0, 0);
    auto fruitCellPosition = getRandomPosition (scene);
    scene->occupiedCells.pop_front ();
    Entity fruitCell = scene->createEntity ("FRUIT",
                                            fruitCellPosition.column * gridCellSize,
                                            fruitCellPosition.row * gridCellSize);
    fruitCell.addComponent<Components::BoxCollider> (
        fruitCellPosition.column, // x offset
        fruitCellPosition.row, // y offset
        scene->gridCellSize, // Width of the box
        scene->gridCellSize, // height of the box
        1.0f, // density
        0.1f // friction
    );
    scene->fruitCell = new Entity (fruitCell);
    scene->grid = rows;
  }
 private :
  int screenWidth;
  int gridSize;
};

struct FruitRender : public Render {
    void run (SDL_Renderer *pRenderer) override
    {
      SDL_SetRenderDrawColor (pRenderer, 255, 30, 30, 1);
      auto fruitTransform = scene->fruitCell->getComponent<Components::Transform> ();
      auto fruitCollider = scene->fruitCell->getComponent<Components::BoxCollider> ();

      auto width = fruitCollider.w;
      auto height = fruitCollider.h;

      auto playerRect = SDL_Rect{fruitTransform.x, fruitTransform.y, width, height};
      SDL_RenderFillRect (pRenderer, &playerRect);

    }
};

struct FruitUpdater : public Update {
    void run (double dT) override
    {
      auto &playerTransform = scene->player->getComponent<Components::Transform> ();
      auto &fruitTransform = scene->fruitCell->getComponent<Components::Transform> ();

      // If they are the same, recalculate the fruit's position and add one
      if (playerTransform.x == fruitTransform.x && playerTransform.y == fruitTransform.y)
        {
          scene->occupiedCells.emplace_back (playerTransform.x, playerTransform.y);
          playerTransform.x = fruitTransform.x;
          playerTransform.y = fruitTransform.y;
          scene->occupiedCells.emplace_back (playerTransform.x, playerTransform.y);
          auto newFruitCellPosition = getRandomPosition (scene);
          scene->occupiedCells.pop_back ();
          auto cellSize = scene->gridCellSize;
          fruitTransform.x = newFruitCellPosition.column * cellSize;
          fruitTransform.y = newFruitCellPosition.row * cellSize;
        }

    }

};

struct CharacterSetup : public Setup {
    CharacterSetup ()
    {};

    ~CharacterSetup () = default;

    void run () override
    {
      auto player = scene->createEntity ("PLAYER", 0, 0);
      auto playerSize = scene->gridCellSize;
      player.addComponent<Components::Movement> (0, 0);
      player.addComponent<Components::BoxCollider> (
          playerSize, // x offset
          playerSize, // y offset
          playerSize, // Width of the box
          playerSize, // height of the box
          1.0f, // density
          0.1f // friction
      );

      scene->player = new Entity (player);
    }

};

class CameraSetup : public Setup {
  int viewportWidth;
  int viewportHeight;
  int worldWidth;
  int worldHeight;
  int zoom;
 public:
  CameraSetup (int z, int vw, int vh, int ww, int wh)
  {
    zoom = z;
    viewportWidth = vw;
    viewportHeight = vh;
    worldWidth = ww;
    worldHeight = wh;
  }
  void run () override
  {
    auto camera = scene->createEntity ("CAMERA", worldWidth / 2, worldHeight / 2);
    camera.addComponent<Components::Camera> (
        zoom,
        viewportWidth,
        viewportHeight,
        worldWidth,
        worldHeight
    );

    scene->mainCamera = new Entity (camera);

  }
};

struct PlayerInput : public Input {
    void run (SDL_Event event) override
    {
      auto &playerMovement = scene->player->getComponent<Components::Movement> ();
      int speed = 1000;
      if (event.type == SDL_KEYDOWN)
        {
          switch (event.key.keysym.sym)
            {
              case SDLK_LEFT:
                {
                  // If it's already going left or right
                  if (playerMovement.vx == 0)
                    {
                      playerMovement.vx = -1;
                      playerMovement.vy = 0;
                    }

                }
              break;
              case SDLK_RIGHT:
                {
                  if (playerMovement.vx == 0)
                    {
                      playerMovement.vx = 1;
                      playerMovement.vy = 0;
                    }
                }
              break;
              case SDLK_UP:
                {
                  if (playerMovement.vy == 0)
                    {
                      playerMovement.vy = -1;
                      playerMovement.vx = 0;
                    }
                }
              break;
              case SDLK_DOWN:
                {
                  if (playerMovement.vy == 0)
                    {
                      playerMovement.vy = 1;
                      playerMovement.vx = 0;
                    }
                }
              break;
            }
        }

    }
}; // PlayerInput

struct MovementUpdate : public Update {

    int moveEveryNFrame;
    int currentFrame;

    MovementUpdate ()
    {
      moveEveryNFrame = 30;
      currentFrame = 0;
    }

    void run (double deltaTime) override
    {
      currentFrame++;
      auto view = scene->r.view<Components::Movement, Components::BoxCollider, Components::Transform> ();
      auto cellSize = scene->gridCellSize;
      if (currentFrame != moveEveryNFrame) return;
      currentFrame = 0;
      for (const entt::entity entity: view)
        {
          auto movement = view.get<Components::Movement> (entity);

          auto &transform = view.get<Components::Transform> (entity);
          scene->occupiedCells.emplace_back (transform.x, transform.y);
          scene->occupiedCells.pop_front ();
          transform.x += movement.vx * cellSize;
          transform.y += movement.vy * cellSize;
        }
    }
};

struct PlayerRender : public Render {
    void run (SDL_Renderer *pRenderer) override
    {
      SDL_SetRenderDrawColor (pRenderer, 225, 100, 100, 1);

      auto playerTransform = scene->player->getComponent<Components::Transform> ();
      auto playerCollider = scene->player->getComponent<Components::BoxCollider> ();

      auto width = playerCollider.w;
      auto height = playerCollider.h;

      auto playerRect = SDL_Rect{playerTransform.x, playerTransform.y, width, height};
      SDL_RenderFillRect (pRenderer, &playerRect);
    }
};

struct PlayerBodyRender : public Render {
    void run (SDL_Renderer *pRenderer) override
    {
      SDL_SetRenderDrawColor (pRenderer, 255, 255, 255, 1);

      for (auto cell: scene->occupiedCells)
        {
          auto size = scene->gridCellSize;
          auto bodySegment = SDL_Rect{cell.row, cell.column, size, size};
          SDL_RenderFillRect (pRenderer, &bodySegment);
        }
    }
};

struct PlayerCollisionUpdate : public Update {
    void run (double dT) override
    {
      auto playerTransform = scene->player->getComponent<Components::Transform> ();
      auto gridSize = scene->gridSize;
      auto cellSize = scene->gridCellSize;
      if (
          playerTransform.x > gridSize * cellSize ||
          playerTransform.y > gridSize * cellSize ||
          playerTransform.x < 0 ||
          playerTransform.y < 0)
        {
          exit (0);
        }
      auto index = 0;
      auto size = scene->occupiedCells.size ();
      for (auto occupiedCell: scene->occupiedCells)
        {
          if (index == size - 1) continue;
          index++;
          if (playerTransform.x == occupiedCell.column && playerTransform.y == occupiedCell.row)
            {
              exit (0);
            }
        }
    }
};

struct BackgroundDecorationRender : public Render {
    void run (SDL_Renderer *pRenderer) override
    {
      auto nextColor = scene->getNextColor ();
      SDL_SetRenderDrawColor (pRenderer, nextColor.r, nextColor.g, nextColor.b, nextColor.a);
      auto decorationSize = scene->gridCellSize;
      for (int rowIndex = 0; rowIndex < scene->grid.size (); ++rowIndex)
        {
          auto row = scene->grid.at (rowIndex);
          for (int columnIndex = 0; columnIndex < row.size (); ++columnIndex)
            {
              auto column = row.at (columnIndex);
              auto x = columnIndex * decorationSize + 1;
              auto y = rowIndex * decorationSize + 1;
              auto decorationRect = SDL_Rect{x, y, decorationSize - 1, decorationSize - 1};
              if (column == 1)
                {
                  SDL_SetRenderDrawColor (pRenderer, 255, 255, 255, 1);
                }
              SDL_RenderFillRect (pRenderer, &decorationRect);
            }
        }
    }
};

struct BackgroundDecorationSetup : public Setup {
    void run () override
    {
      scene->direction = 1;
      scene->currentColorIndex = 0;
      scene->changeEveryFrames = 120;
      scene->currentFrame = 0;

      Uint8 full = 255;
      scene->colors.emplace_back (255, 167, 0, 1);
      scene->colors.emplace_back (255, 170, 29, (Uint8) ((float) full * 0.8f));
      scene->colors.emplace_back (255, 211, 0, (Uint8) ((float) full * 0.6f));
      scene->colors.emplace_back (238, 210, 2, (Uint8) ((float) full * 0.4f));
      scene->colors.emplace_back (239, 204, 0, (Uint8) ((float) full * 0.2f));
    }
};
#endif //SYSTEMIMPL_HPP_
