#include "player.h"
#include "comm/log_def.h"
#include "core/scene.h"
#include <SDL3/SDL.h>
#include "resource_mgr.h"
#include "affiliate/sprite_anim.h"
#include "affiliate/collider.h"
#include "raw/stats.h"
#include "world/effect.h"
#include "weapon_thunder.h"
#include "affiliate/bar.h"
#include "screen/hud_stats.h"

s32 Player::init()
{
    Actor::init();

    max_speed_ = 400;

    sprite_idle_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghost-idle.png",
        Anchor::CENTER,
        2.0f
    );

    sprite_move_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghost-move.png",
        Anchor::CENTER,
        2.0f
    );
    sprite_move_->set_active(false);


    // 添加碰撞器
    auto collider = Collider::add_collider(
        this,
        Collider::Type::CIRCLE,
        Vec2{sprite_idle_->get_size().x * 0.6f}, // 直径
        Anchor::CENTER
    );


    auto* stats = Stats::add_stats(
        this,
        100.0f,
        100.0f,
        50.0f,
        50.0f,
        10.0f,
        5.0f
    );


    // parent为nullptr, 在死亡的时候才会添加到scene
    effect_dead_ = Effect::add_effect(
        nullptr,
        "../assets/effect/1764.png",
        Vec2{0},
        nullptr,
        3.0f
    );


    // 添加雷武
    Weapon::add_weapon<WeaponThunder>(
        this,
        1000,
        15    // 魔法消耗15
    );


    // 血条
    Bar<f32>::add_bar(
        this,
        stats->get_ptr_hp(),
        stats->get_ptr_max_hp(),
        Vec2{collider->get_size().x, 10},
        Vec2{collider->get_size().x} * Vec2{-0.5f, 0.5f},
        DEFAULT_HP_BAR_COLOR_SEQ
    );

    // 魔法条
    Bar<f32>::add_bar(
        this,
        stats->get_ptr_mana(),
        stats->get_ptr_max_mana(),
        Vec2{collider->get_size().x, 10},
        Vec2{collider->get_size().x} * Vec2{-0.5f, 0.5f} + Vec2{0, 12},
        DEFAULT_MANA_BAR_COLOR_SEQ
    );


    // 添加HUD
    HudStats::add_hud_stats(
        game_.get_curr_scene(),
        this,
        {
            "../assets/UI/bar_red.png",     // hp
            "../assets/UI/bar_bg.png",      // hp bg
            "../assets/UI/Red Potion.png",  // hp icon
            "../assets/UI/bar_blue.png",    // mana
            "../assets/UI/bar_bg.png",      // mana bg
            "../assets/UI/Blue Potion.png"  // mana icon
        },
        stats->get_ptr_hp(),
        stats->get_ptr_max_hp(),
        stats->get_ptr_mana(),
        stats->get_ptr_max_mana()
    );

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
    
    check_state();

    move(delta_ms);
    sync_camera();

    check_is_dead();

    return 0;
}

s32 Player::render()
{
    Actor::render();

#ifdef DWT_DEBUG
    auto aabb = game_.GetCameraAABB();

    LOG_TRACE("camera aabb=[{},{},{},{}], player pos=[{},{}]",
             aabb.first.x, aabb.first.y, aabb.second.x, aabb.second.y,
             pos().x, pos().y);

    game_.draw_rect(get_render_pos(), {5, 5}, Color(255, 0, 0, 255));
#endif

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

    static const std::vector move_keys_vec = {
        SDL_SCANCODE_W, SDL_SCANCODE_UP,
        SDL_SCANCODE_A, SDL_SCANCODE_LEFT,
        SDL_SCANCODE_S, SDL_SCANCODE_DOWN,
        SDL_SCANCODE_D, SDL_SCANCODE_RIGHT
    };

    auto has_key = [&]()
    {
        for (auto& t : move_keys_vec)
        {
            if (key_state[t]) return true;
        }
        return false;
    };

    if (has_key())
    {
        velocity_ = Vec2{0};

        if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP])
        {
            // move up
            velocity_.y = -1;
        }
        if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT])
        {
            // move left
            velocity_.x = -1;
        }
        if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN])
        {
            // move down
            velocity_.y = 1;
        }
        if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT])
        {
            // move right
            velocity_.x = 1;
        }

        velocity_ = glm::normalize(velocity_) * max_speed_;

        LOG_TRACE("v=[{}, {}]", velocity_.x, velocity_.y);
    }
    


    

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

s32 Player::check_state()
{
    if (velocity_.x < 0)
    {
        sprite_idle_->get_texture().is_flip = true;
        sprite_move_->get_texture().is_flip = true;
    }
    else if (velocity_.x >= 0)
    {
        sprite_idle_->get_texture().is_flip = false;
        sprite_move_->get_texture().is_flip = false;
    }

    bool moving = (glm::length(velocity_) > 0.1f);

    if (moving != is_moving_)
    {
        change_state(moving);
    }


    return 0;
}

s32 Player::change_state(bool moving)
{
    is_moving_ = moving;
    if (moving)
    {
        sprite_idle_->set_active(false);
        sprite_move_->set_active(true);
        sprite_move_->copy_state(*sprite_idle_);
    }
    else
    {
        sprite_move_->set_active(false);
        sprite_idle_->set_active(true);
        sprite_idle_->copy_state(*sprite_move_);
    }

    return 0;
}

s32 Player::check_is_dead()
{
    if (! is_alive())
    {
        auto scene = G_GAME.get_curr_scene();
        if (scene)
        {
            effect_dead_->set_pos(pos());
            scene->add_child_safe(effect_dead_);
            set_active(false);
        }
    }

    return 0;
}
