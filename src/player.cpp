#include "player.h"
#include "comm/log_def.h"
#include "core/scene.h"
#include <SDL3/SDL.h>
#include "resource_mgr.h"

s32 Player::init()
{
    Actor::init();

    max_speed_ = 400;
    return 0;
}

s32 Player::handle_events(SDL_Event &event)
{
    Actor::handle_events(event);

    return 0;
}

s32 Player::update(s64 now_ms, s64 delta_ms)
{
    Actor::update(now_ms, delta_ms);

    keyboard_control();
    move(delta_ms);
    sync_camera();
    return 0;
}

s32 Player::render()
{
    Actor::render();

    auto* ttt = G_RESOURCE_MGR.loadResource<SDL_Texture>("../assets/sprite/ghost-idle.png");

    auto aabb = game_.GetCameraAABB();

    LOG_TRACE("camera aabb=[{},{},{},{}], player pos=[{},{}]",
             aabb.first.x, aabb.first.y, aabb.second.x, aabb.second.y,
             pos().x, pos().y);

    game_.draw_rect(get_render_pos() - 10.0f, {20, 20}, Color(255, 0, 0, 255));

    return 0;
}

s32 Player::clean()
{
    Actor::clean();

    return 0;
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

    return 0;
}

s32 Player::move(s64 delta_ms)
{
    auto p = pos();
    p += velocity_ * (delta_ms / 1000.0f);

    // 限制移动
    auto* scene = game_.get_curr_scene();
    if (scene)
    {
        p = glm::clamp(p, Vec2(0), scene->get_world_size() - 20.0f);
    }

    set_world_pos(p);
    
    return 0;
}

s32 Player::sync_camera()
{
    constexpr f32 camera_out_margin = 60;

    auto* scene = game_.get_curr_scene();
    if (scene)
    {
        auto p = pos() - game_.get_screen_size() / 2.0f;
        p = glm::clamp(p,
            Vec2(-camera_out_margin),
            Vec2(camera_out_margin) + scene->get_world_size() - game_.get_screen_size()
        );
        scene->set_camera_pos(p);
    }

    return 0;
}
