#pragma once


#include "core/actor.h"


class Player;
class SpriteAnim;

class Enemy : public Actor
{


public:

    s32 init() override;
    // s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    // s32 clean() override;


    s32 aim_target(const Actor& actor);

    void set_target(Actor* target) { target_ = target; }
    Actor* get_target() const { return target_; }

protected:
    Actor* target_ = nullptr;

    SpriteAnim* sprite_ = nullptr;
};