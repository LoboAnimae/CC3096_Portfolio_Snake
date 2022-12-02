//
// Created by yagdrassyl on 1/12/22.
//

#ifndef ENTITIES_HPP_
#define ENTITIES_HPP_
#include <entt/entt.hpp>
#include "Scene.hpp"
struct Entity {

  Entity (entt::entity entity, Scene *pScene)
  {
    handle = entity;
    scene = pScene;
  }

  Entity (const Entity &copy)
  {
    handle = copy.handle;
    scene = copy.scene;
  }
  ~Entity () = default;

  template<class T, typename ...Args>
  T &addComponent (Args &&... args)
  {
    return scene->r.emplace<T> (handle, std::forward<Args> (args)...);
  }

  template<typename T>
  void removeComponent ()
  {
    scene->r.remove<T> (handle);
  }

  template<typename T>
  T &getComponent ()
  {
    return scene->r.get<T> (handle);
  }

  entt::entity handle;
 private:
  Scene *scene;
}; // Entity

#endif //_ENTITIES_HPP_
