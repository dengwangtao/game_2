#include "enemy.h"

#include "player.h"
#include "comm/log_def.h"
#include "core/scene.h"

#include "affiliate/sprite_anim.h"
#include "affiliate/collider.h"
#include "raw/stats.h"

s32 Enemy::init()
{
    Actor::init();

    sprite_normal_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghost-Sheet.png",
        Anchor::CENTER,
        2.0f
    );

    sprite_hurt_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghostHurt-Sheet.png",
        Anchor::CENTER,
        2.0f,
        false
    );
    sprite_hurt_->set_loop(false);

    sprite_die_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghostDead-Sheet.png",
        Anchor::CENTER,
        2.0f,
        false
    );
    sprite_die_->set_loop(false);

    sprite_current_ = sprite_normal_;


    // 添加碰撞器
    auto collider = Collider::add_collider(
        this,
        Collider::Type::CIRCLE,
        Vec2{sprite_current_->get_size().x * 0.9f}, // 直径
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

    return 0;
}

s32 Enemy::update(s64 now_ms, s64 delta_ms)
{
  Actor::update(now_ms, delta_ms);

  if (target_)
  {
    aim_target(*target_);
  }

  move(delta_ms);

  attack();

  return 0;
}

s32 Enemy::render()
{
    Actor::render();

#ifdef DWT_DEBUG
    game_.draw_rect(get_render_pos(), {5, 5}, Color(0, 255, 0, 255));
#endif

    return 0;
}

s32 Enemy::destroy()
{
    if (sprite_die_->is_finished())
    {
        mark_for_delete(); // 标记删除
    }

    return 0;
}

s32 Enemy::attack()
{
    if (collider_ && target_->get_collider())
    {
        if (collider_->is_colliding(*target_->get_collider()))
        {
            LOG_TRACE("Enemy attack hit the target!");
            
            if (stats_ && target_->get_stats())
            {
                target_->take_damage(stats_->get_attack(), this);
            }

        }
    }

    return 0;
}

s32 Enemy::aim_target(const Actor& actor) {
  auto dir = actor.get_world_pos() - get_world_pos();
  dir = glm::normalize(dir);

  set_velocity(dir * max_speed_);

  return 0;
}

s32 Enemy::check_state()
{
    // TODO:

    return 0;
}

s32 Enemy::change_state(State state)
{
    if (state_ == state)
    {
        return 0;
    }

    sprite_current_->set_active(false);

    switch (state)
    {
        case State::NORMAL:
        {
            sprite_current_ = sprite_normal_;
            break;
        }
        case State::HURT:
        {
            sprite_current_ = sprite_hurt_;
            break;
        }
        case State::DIE:
        {
            sprite_current_ = sprite_die_;
            break;
        }
        default:
            break;
    }

    state_ = state;
    sprite_current_->set_active(true);
    return 0;
}