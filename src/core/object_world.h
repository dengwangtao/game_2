#pragma once

#include "core/object_screen.h"
#include "comm/comm_def.h"

class ObjectWorld : public ObjectScreen
{
public:

    s32 update(s64 now_ms) override;

    void set_world_pos(const Vec2& pos);
    const Vec2& get_world_pos() const { return world_pos_; }
    const Vec2& pos() const { return world_pos_; }

    // 重写ObjectScreen的set_render_pos
    void set_render_pos(const Vec2& pos) override;

protected:
    Vec2 world_pos_; // 世界位置
};