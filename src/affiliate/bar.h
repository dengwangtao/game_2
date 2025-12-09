#pragma once


#include <concepts>
#include "core/object_affiliate.h"
#include "core/actor.h"
#include "comm/comm_def.h"
#include "comm/log_def.h"


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

        G_GAME.draw_rect(
            pos + Vec2{1.0f, 1.0f},
            (get_size() - Vec2{2.0f, 2.0f}) * ((*value_) / (*max_value_)),
            Color{0, 255, 0, 255}
        );

        return 0;
    }
    std::string to_string() const override
    { return "Bar:" + std::format("{:x}", (uintptr_t)this); }


    void set_owner(Actor* owner) { owner_ = owner; }
    Actor* get_owner() const { return owner_; }

    void set_value_ptr(T* value) { value_ = value; }
    void set_max_value_ptr(T* max_value) { max_value_ = max_value; }


    static Bar<T>* add_bar(
        Actor* owner,
        T* value,
        T* max_value,
        const Vec2& size,
        const Vec2& offset
    )
    {
        auto* bar = new Bar<T>();
        bar->init();

        bar->set_owner(owner);
        bar->set_value_ptr(value);
        bar->set_max_value_ptr(max_value);
        bar->set_size(size);
        bar->set_offset(offset);

        if(owner) owner->add_child(bar);

        return bar;
    }

private:
    Actor* owner_ = nullptr; // 所属的角色
    T* value_ = nullptr; // 指向角色的某个属性值
    T* max_value_ = nullptr; // 指向角色的某个属性最大值
};


