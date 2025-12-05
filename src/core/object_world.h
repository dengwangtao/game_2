#pragma once

#include "core/object_screen.h"
#include "comm/comm_def.h"


class Collider;

class ObjectWorld : public ObjectScreen
{
public:

    s32 update(s64 now_ms, s64 delta_ms) override;

    void set_world_pos(const Vec2& pos);
    void set_pos(const Vec2& pos) { set_world_pos(pos); }
    const Vec2& get_world_pos() const { return world_pos_; }
    Vec2 pos() const override { return world_pos_; }

    // 重写ObjectScreen的set_render_pos
    void set_render_pos(const Vec2& pos) override;

    Collider* get_collider() const { return collider_; }
    void set_collider(Collider* collider) { collider_ = collider; }

protected:
    Vec2 world_pos_; // 世界位置
    Collider* collider_ = nullptr; // 关联的碰撞器
};