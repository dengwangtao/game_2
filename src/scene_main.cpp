#include "scene_main.h"
#include "player.h"

#include "enemy.h"
#include "world/effect.h"
#include "screen/ui_mouse.h"
#include "comm/log_def.h"
#include "world/spell.h"
#include "screen/hud_text.h"

s32 SceneMain::init()
{
    Scene::init();
    
    world_size_ = game_.get_screen_size() * 3.0f;

    camera_pos_ = world_size_ / 2.0f - game_.get_screen_size() / 2.0f; // 相机位置也放在最中间

    player_ = new Player();
    player_->init();
    player_->set_pos(world_size_ / 2.0f);
    add_child(player_);

    spawner_ = new Spawner();
    spawner_->set_target_player(player_);
    add_child(spawner_);

    SDL_HideCursor(); // 隐藏系统鼠标光标

    UIMouse::add_ui_mouse(
        this,
        "../assets/UI/29.png",
        "../assets/UI/30.png"
    );


    Vec2 hud_text_size{300.0f, 50.0f};

    HUDText::add_hud_text(
        this,
        "文本测试",
        Vec2{
            game_.get_screen_size().x - hud_text_size.x / 2.0f - 20,
            20 + hud_text_size.y / 2.0f
        },
        hud_text_size,
        "../assets/font/VonwaonBitmap-16px.ttf",
        20,
        "../assets/UI/Textfield_01.png",
        Anchor::CENTER
    );

    return 0;
}

s32 SceneMain::handle_events(SDL_Event& event)
{
    Scene::handle_events(event);
    
    return 0;
}

s32 SceneMain::update(s64 now_ms, s64 delta_ms)
{
    Scene::update(now_ms, delta_ms);

    return 0;
}

s32 SceneMain::render()
{
    render_background();
    
    Scene::render();
    
    return 0;
}

s32 SceneMain::clean()
{
    Scene::clean();

    return 0;
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
