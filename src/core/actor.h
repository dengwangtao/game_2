#pragma once

#include "core/object_world.h"
#include "comm/comm_def.h"


class Stats;

class Actor : public ObjectWorld
{

public:

    s32 move(s64 delta_ms);

    s32 take_damage(f32 damage, Actor* attacker);


    bool is_alive() const;

    void set_velocity(const Vec2& velocity) { velocity_ = velocity; }
    const Vec2& get_velocity() const { return velocity_; }
    void set_max_speed(f32 max_speed) { max_speed_ = max_speed; }
    f32 get_max_speed() const { return max_speed_; }

    void set_stats(Stats* stats) { stats_ = stats; }
    Stats* get_stats() const { return stats_; }

protected:
    Vec2 velocity_; // 速度
    f32 max_speed_= 100.0f; // 最大速度
    Stats* stats_ = nullptr; // 角色属性

};