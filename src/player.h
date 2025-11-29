#pragma once


#include "core/actor.h"


class Player : public Actor
{

public:
    s32 init() override;
    s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    s32 clean() override;

    s32 keyboard_control();
    s32 move(s64 delta_ms);
    s32 sync_camera(); // 相机跟随
};