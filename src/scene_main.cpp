#include "scene_main.h"
#include "player.h"

s32 SceneMain::init()
{
    world_size_ = game_.get_screen_size() * 3.0f;

    camera_pos_ = world_size_ / 2.0f - game_.get_screen_size() / 2.0f; // 相机位置也放在最中间

    player_ = new Player();
    player_->init();
    player_->set_pos(world_size_ / 2.0f);
    return 0;
}

s32 SceneMain::handle_events(SDL_Event& event)
{
    return s32();
}

s32 SceneMain::update(s64 now_ms, s64 delta_ms)
{
    // TODO: 待删除
    // camera_pos_ += Vec2{1, 1};

    player_->update(now_ms, delta_ms);

    return s32();
}

s32 SceneMain::render()
{
    render_background();
    
    player_->render();

    return s32();
}

s32 SceneMain::clean()
{
    player_->clean();
    delete player_;
    player_ = nullptr;

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
        Color{127, 127, 127, 255}
    );

    game_.draw_boundary(
        start,
        end,
        10,
        Color{255, 255, 255, 255}
    );

    return 0;
}
