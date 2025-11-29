#include "scene_main.h"

s32 SceneMain::init()
{
    world_size_ = game_.get_screen_size() * 3.0f;
    return 0;
}

s32 SceneMain::handle_events(SDL_Event& event)
{
    return s32();
}

s32 SceneMain::update(s64 now_ms)
{
    camera_pos_ += Vec2{4, 4};

    return s32();
}

s32 SceneMain::render()
{
    render_background();


    return s32();
}

s32 SceneMain::clean()
{
    return s32();
}

s32 SceneMain::render_background()
{
    auto start = - camera_pos_;
    auto end = world_size_ - camera_pos_;

    game_.draw_grid(
        start,
        end,
        Vec2{80, 80},
        Color{255, 255, 255, 255}
    );

    return 0;
}
