#include "actor.h"
#include "comm/comm_def.h"
#include "scene_main.h"
#include "raw/stats.h"
#include "comm/log_def.h"

s32 Actor::move(s64 delta_ms)
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

s32 Actor::take_damage(f32 damage, Actor* attacker)
{
    if (stats_)
    {
        s32 ret = stats_->take_damage(damage);
        if (ret == 0)
        {
            LOG_DEBUG("Actor {} took {} damage from Actor {}", to_string(), damage, attacker ? attacker->to_string() : "null");
        }
    }

    return 0;
}

bool Actor::is_alive() const
{
    if (stats_)
    {
        return stats_->get_is_alive();
    }
    return true;
}
