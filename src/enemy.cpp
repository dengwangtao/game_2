#include "enemy.h"

#include "player.h"
#include "comm/log_def.h"

#include "affiliate/sprite_anim.h"

s32 Enemy::init()
{
    Actor::init();

    sprite_ = Sprite::add_sprite<SpriteAnim>(
        this,
        "../assets/sprite/ghost-Sheet.png",
        2.0f
    );

    return 0;
}

s32 Enemy::update(s64 now_ms, s64 delta_ms) {
  Actor::update(now_ms, delta_ms);

  if (target_) {
    aim_target(*target_);
  }

  move(delta_ms);

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

s32 Enemy::aim_target(const Actor& actor)
{
  auto dir = actor.get_world_pos() - get_world_pos();
  dir = glm::normalize(dir);

  set_velocity(dir * max_speed_);

  return 0;
};