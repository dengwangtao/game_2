#pragma once

#include "core/object.h"
#include "comm/comm_def.h"

class ObjectScreen : public Object
{
public:
    s32 render() override;


    virtual void set_render_pos(const Vec2& pos) { render_pos_ = pos; }
    const Vec2& get_render_pos() const { return render_pos_; }

    virtual Vec2 pos() const { return Vec2{0}; }

protected:
    Vec2 render_pos_; // 屏幕位置/渲染位置
};