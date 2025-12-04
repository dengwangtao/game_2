#pragma once

#include "core/object_world.h"
#include "comm/comm_def.h"


class Actor : public ObjectWorld
{

public:

    s32 move(s64 delta_ms);

    void set_velocity(const Vec2& velocity) { velocity_ = velocity; }
    const Vec2& get_velocity() const { return velocity_; }
    void set_max_speed(f32 max_speed) { max_speed_ = max_speed; }
    f32 get_max_speed() const { return max_speed_; }



protected:
    Vec2 velocity_; // 速度
    f32 max_speed_= 100.0f; // 最大速度

};