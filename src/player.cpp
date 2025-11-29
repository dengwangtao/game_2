#include "player.h"
#include "comm/log_def.h"
#include "core/scene.h"
#include <SDL3/SDL.h>

s32 Player::init()
{
    max_speed_ = 200;
    return s32();
}

s32 Player::handle_events(SDL_Event &event)
{
    return s32();
}

s32 Player::update(s64 now_ms, s64 delta_ms)
{
    keyboard_control();
    move(delta_ms);
    sync_camera();
    return s32();
}

s32 Player::render()
{
    auto aabb = game_.GetCameraAABB();

    LOG_TRACE("camera aabb=[{},{},{},{}], player pos=[{},{}]",
             aabb.first.x, aabb.first.y, aabb.second.x, aabb.second.y,
             pos().x, pos().y);

    game_.draw_rect(get_render_pos() - 10.0f, {20, 20}, Color(255, 0, 0, 255));

    return s32();
}

s32 Player::clean()
{
    return s32();
}

s32 Player::keyboard_control()
{
    velocity_ *= 0.9; // 速度衰减

    auto key_state = SDL_GetKeyboardState(nullptr);
    if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP])
    {
        // move up
        velocity_.y = -max_speed_;
    }
    if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT])
    {
        // move left
        velocity_.x = -max_speed_;
    }
    if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN])
    {
        // move down
        velocity_.y = max_speed_;
    }
    if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT])
    {
        // move right
        velocity_.x = max_speed_;
    }

    return s32();
}

s32 Player::move(s64 delta_ms)
{
    auto p = pos();
    p += velocity_ * (delta_ms / 1000.0f);
    set_world_pos(p);
    
    return s32();
}

s32 Player::sync_camera()
{
    auto* scene = game_.get_curr_scene();
    if (scene)
    {
        scene->set_camera_pos(pos() - game_.get_screen_size() / 2.0f);
    }

    return s32();
}
