//
// Created by yagdrassyl on 1/12/22.
//
#include "Entities.hpp"
#include "Scene.hpp"
#include "Components.hpp"
#include "Systems.hpp"

Entity Scene::createEntity (const std::string &name, int x, int y)
{
  Entity entity = {r.create (), this};
  entity.addComponent<Components::Tag> ();
  entity.addComponent<Components::Transform> (x, y);
  return entity;
}

Entity Scene::createEntity (const std::string &name, b2BodyType type, int x, int y)
{
  Entity entity = {r.create (), this};
  entity.addComponent<Components::Tag> (name);
  entity.addComponent<Components::Transform> (x, y);

  b2BodyDef bodyDef;
  bodyDef.type = type;
  bodyDef.position.Set ((float) x, (float) y);
  b2Body *body = world->CreateBody (&bodyDef);
  body->SetFixedRotation (true);

  struct BodyUserData {
      explicit BodyUserData (const Entity &e) : entity (e)
      {};
      Entity entity;
  };

  auto *data = new BodyUserData (entity);
  body->GetUserData ().pointer = reinterpret_cast<uintptr_t>(data);

  entity.addComponent<Components::RigidBody> (type, body);
  return entity;

}

void Scene::addSetupSystem (Setup *newSystem)
{
  newSystem->setScene (this);
  setupSystems.push_back (newSystem);
}

void Scene::addInputSystem (Input *newSystem)
{
  newSystem->setScene (this);
  inputSystems.push_back (newSystem);
}

void Scene::addUpdateSystem (Update *newSystem)
{
  newSystem->setScene (this);
  updateSystems.push_back (newSystem);
}

void Scene::addRenderSystem (Render *newSystem)
{
  newSystem->setScene (this);
  renderSystems.push_back (newSystem);
}

void Scene::setup ()
{

  for (Setup *sys: setupSystems)
    {
      sys->run ();
    }
}

void Scene::input (SDL_Event event)
{
  for (Input *sys: inputSystems)
    {
      sys->run (event);
    }
}

void Scene::update (double dT)
{

  for (Update *sys: updateSystems)
    {
      sys->run (dT);
    }
}

void Scene::render (SDL_Renderer *renderer)
{

  for (Render *sys: renderSystems)
    {
      sys->run (renderer);
    }
}
Scene::Scene (const std::string &)
{
  collisionEvent = SDL_RegisterEvents (1);

  world = new b2World ({0.0f, 9.8f * 100});
//  world->SetContactListener (new ContactListener (collisionEvent));
}
Color Scene::getNextColor ()
{
  if (currentFrame != changeEveryFrames - 1)
    {
      currentFrame++;
      return colors.at (currentColorIndex);
    }
  currentFrame = 0;
  currentColorIndex = direction + currentColorIndex;
  if (currentColorIndex > colors.size () - 1 || currentColorIndex < 0)
    {
      direction *= -1;
      currentColorIndex = 2 * direction + currentColorIndex;
    }

  return colors.at (currentColorIndex);
}
