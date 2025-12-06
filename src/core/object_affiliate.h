#pragma once

#include "core/object_screen.h"
#include "comm/comm_def.h"

class ObjectAffiliate : public Object
{

public:
    // s32 init() override;
    // s32 handle_events(SDL_Event& event) override;
    // s32 update(s64 now_ms, s64 delta_ms) override;
    // s32 render() override;
    // s32 clean() override;


    void set_parent(ObjectScreen* parent) { parent_ = parent; }
    ObjectScreen* get_parent() const { return parent_; }
    void set_offset(const Vec2& offset) { offset_ = offset; }
    const Vec2& get_offset() const { return offset_; }
    void set_size(const Vec2& size);
    const Vec2& get_size() const { return size_; }

    void set_offset_by_anchor(Anchor anchor);
    void scale(f32 factor)
    {
        set_size(size_ * factor);
    }

protected:
    ObjectScreen* parent_ = nullptr;
    Vec2 offset_ = Vec2{0};
    Vec2 size_ = Vec2{0};
    Anchor anchor_ = Anchor::TOP_LEFT; // 锚点
};