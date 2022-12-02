//
// Created by yagdrassyl on 1/12/22.
//

#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_
#include "Scene.hpp"

struct System {
  void setScene (Scene *newScene)
  {
    scene = newScene;
  }
 protected:
  Scene *scene;
};

class Setup : public System {
 public:
  virtual void run () = 0;
};

class Input : public System {
 public:
  virtual void run (SDL_Event event) = 0;
};

class Update : public System {
 public:
  virtual void run (double dT) = 0;
};

class Render : public System {
 public:
  virtual void run (SDL_Renderer *renderer) = 0;
};

#endif //SYSTEMS_HPP_
