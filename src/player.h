#pragma once


#include "core/actor.h"


class SpriteAnim;

class Player : public Actor
{

public:
    s32 init() override;
    s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    s32 clean() override;

    s32 keyboard_control();
    s32 sync_camera(); // 相机跟随
    s32 check_state();
    s32 change_state(bool moving);

protected:
    SpriteAnim* sprite_idle_ = nullptr;
    SpriteAnim* sprite_move_ = nullptr;
    bool is_moving_ = false;
};