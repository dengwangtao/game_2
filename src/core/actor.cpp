#include "actor.h"
#include "comm/comm_def.h"
#include "scene_main.h"

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