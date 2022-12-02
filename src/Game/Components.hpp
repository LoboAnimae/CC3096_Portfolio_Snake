//
// Created by yagdrassyl on 1/12/22.
//

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_
#include <string>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>

namespace Components
{
    struct Tag {
        std::string tag;
    };

    struct Transform {
        int x = 0;
        int y = 0;
    };

    struct Movement {
        int vx = 0;
        int vy = 0;
    };

    struct Camera {
        int zoom = 1;
        int vw = 0;
        int vh = 0;
        int ww = 0;
        int wh = 0;
    };

    struct RigidBody {
        b2BodyType type = b2_staticBody;
        b2Body *body = nullptr;
    };

    struct BoxCollider {
        int xo = 0;
        int yo = 0;
        int w = 0;
        int h = 0;
        float density = 1.f;
        float friction = 0.1f;
        float restitution = 0.0f;
        float restitutionThreshold = 0.0f;
        SDL_Color color = {255, 255, 255};
    };
}

#endif //COMPONENTS_HPP_
