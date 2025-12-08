#include "weapon.h"
#include "comm/log_def.h"
#include "world/spell.h"
#include "core/scene.h"
#include "core/actor.h"
#include "raw/stats.h"

s32 Weapon::update(s64 now_ms, s64 delta_ms)
{
    Object::update(now_ms, delta_ms);

    cooldown_timer_ += delta_ms;

    return 0;
}

bool Weapon::can_attack() const
{

    if (cooldown_timer_ < cooldown_duration_)
    {
        return false;
    }

    if (!owner_)
    {
        LOG_ERROR("Weapon has no owner!");
        return false;
    }

    auto* owner_stats = owner_->get_stats();
    if (!owner_stats)
    {
        LOG_ERROR("Weapon owner has no stats!");
        return false;
    }

    if (!owner_stats->can_use_mana((f32)mana_cost_))
    {
        return false;
    }

    return true;
}

s32 Weapon::attack(const Vec2& world_pos, Spell* spell)
{
    if (! spell)
    {
        LOG_ERROR("Spell is null!");
        return -1;
    }

    if (!can_attack())
    {
        LOG_ERROR("Weapon cannot attack now!");
        return -1;
    }

    // 扣除法力
    owner_->get_stats()->use_mana((f32)mana_cost_);

    // 攻击之后重置冷却计时器
    cooldown_timer_ = 0;

    spell->set_pos(world_pos);
    G_GAME.get_curr_scene()->add_child_safe(spell);

  return 0;
}
