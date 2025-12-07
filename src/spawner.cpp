#include "spawner.h"
#include "comm/tool.h"
#include "enemy.h"
#include "comm/log_def.h"
#include "core/scene.h"

s32 Spawner::update(s64 now_ms, s64 delta_ms)
{
    // Tool::random_range<s32>(1, 100);

    timer_ += delta_ms;

    if (timer_ >= interval_)
    {
        timer_ = 0;

        auto* scene = G_GAME.get_curr_scene();
        if (! scene)
        {
            LOG_ERROR("has no curr scene, spawn enemy failed");
            return -1;
        }

        for (s32 i = 0; i < num_; ++ i)
        {
            auto pos = Tool::random_range<Vec2>(
                scene->get_camera_pos(),
                scene->get_camera_pos() + G_GAME.get_screen_size()
            );
            
            Enemy::add_enemy(scene, pos, target_player_);
        }

    }


    return 0;
}