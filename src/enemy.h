#pragma once


#include "core/actor.h"


class Player;
class SpriteAnim;

class Enemy : public Actor
{
    enum class State
    {
        NORMAL,
        HURT,
        DIE
    };

public:

    s32 init() override;
    // s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    // s32 clean() override;
    std::string to_string() const override
    { return "Enemy:" + std::format("{:x}", (uintptr_t)this); }

    s32 destroy();

    s32 attack();
    s32 aim_target(const Actor& actor);

    s32 check_state();
    s32 change_state(State state);




    void set_target(Actor* target) { target_ = target; }
    Actor* get_target() const { return target_; }



    static Enemy* add_enemy(
        Object* parent,
        const Vec2& pos,
        Player* player
    );

protected:
    Actor* target_ = nullptr;

    State state_ = State::NORMAL;
    SpriteAnim* sprite_normal_ = nullptr;
    SpriteAnim* sprite_hurt_ = nullptr;
    SpriteAnim* sprite_die_ = nullptr;
    SpriteAnim* sprite_current_ = nullptr;
};