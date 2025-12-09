#pragma once


#include <concepts>
#include "core/object_affiliate.h"
#include "core/actor.h"
#include "comm/comm_def.h"
#include "comm/log_def.h"


struct BarColorSeq
{
    struct ColorPoint
    {
        f32 ratio; // 0.0 ~ 1.0, <=ratio就使用该颜色
        Color color;
    };

    std::vector<ColorPoint> seq;
};

const BarColorSeq DEFAULT_HP_BAR_COLOR_SEQ = {
    {
        {0.10f, Color{220, 20, 60, 255}},   // 深红（濒死）
        {0.25f, Color{255, 69, 0, 255}},    // 亮红
        {0.40f, Color{255, 165, 0, 255}},   // 橙色
        {0.60f, Color{255, 215, 0, 255}},   // 金黄
        {0.80f, Color{173, 255, 47, 255}},  // 黄绿
        {1.00f, Color{0, 200, 0, 255}},     // 标准绿
    }
};

const BarColorSeq DEFAULT_MANA_BAR_COLOR_SEQ = {
    {
        {0.10f, Color{135, 206, 250, 255}}, // 浅天蓝
        {0.25f, Color{100, 149, 237, 255}}, // 亮蓝
        {0.40f, Color{65, 105, 225, 255}},  // 皇家蓝
        {0.60f, Color{0, 0, 255, 255}},     // 纯蓝
        {0.80f, Color{0, 0, 205, 255}},     // 深蓝
        {1.00f, Color{25, 25, 112, 255}},   // 午夜蓝
    }
};

template <typename T>
requires std::is_arithmetic_v<T>
class Bar : public ObjectAffiliate
{

public:
    s32 render() override
    {
        ObjectAffiliate::render();

        if (! owner_)
        {
            LOG_WARN("Bar has no owner!");
            return -1;
        }
        
        auto pos = owner_->get_render_pos() + offset_;

        G_GAME.draw_boundary(
            pos,
            pos + get_size(),
            1.0f,
            Color{255, 255, 255, 255}
        );

        f32 ratio = (*value_) / (*max_value_);
        Color color = choose_color(ratio);

        G_GAME.draw_rect(
            pos + Vec2{1.0f, 1.0f},
            (get_size() - Vec2{2.0f, 2.0f}) * Vec2{ratio, 1.0f},
            color
        );

        return 0;
    }

    Color choose_color(f32 ratio) const
    {
        Color c{0, 255, 0, 255}; // 默认绿色

        for (const auto& point : color_seq_.seq)
        {
            if (ratio <= point.ratio)
            {
                return point.color;
            }
        }
        return c;
    }

    std::string to_string() const override
    { return "Bar:" + std::format("{:x}", (uintptr_t)this); }


    void set_owner(Actor* owner) { owner_ = owner; }
    Actor* get_owner() const { return owner_; }

    void set_value_ptr(T* value) { value_ = value; }
    void set_max_value_ptr(T* max_value) { max_value_ = max_value; }

    void set_color_seq(const BarColorSeq& color_seq) { color_seq_ = color_seq; }


    static Bar<T>* add_bar(
        Actor* owner,
        T* value,
        T* max_value,
        const Vec2& size,
        const Vec2& offset,
        const BarColorSeq& color_seq = DEFAULT_HP_BAR_COLOR_SEQ
    )
    {
        auto* bar = new Bar<T>();
        bar->init();

        bar->set_owner(owner);
        bar->set_value_ptr(value);
        bar->set_max_value_ptr(max_value);
        bar->set_size(size);
        bar->set_offset(offset);
        bar->set_color_seq(color_seq);

        if(owner) owner->add_child(bar);

        return bar;
    }

private:
    Actor* owner_ = nullptr; // 所属的角色
    T* value_ = nullptr; // 指向角色的某个属性值
    T* max_value_ = nullptr; // 指向角色的某个属性最大值
    BarColorSeq color_seq_;
};


