#pragma once



#include "core/object_affiliate.h"


class ObjectWorld;

class Collider : public ObjectAffiliate
{
public:
    enum class Type
    {
        CIRCLE, // size_.x is 直径
        RECT, // size_ is width and height
    };

    s32 render() override;

    bool is_colliding(const Collider& other) const;
    bool is_aabb_colliding(const Collider& other) const; // 宽相交叉盒碰撞检测
    std::tuple<Vec2, Vec2, bool> get_aabb(bool render=false) const;


    Type get_type() const { return type_; }
    void set_type(Type type) { type_ = type; }



    static s32 add_collider(
        ObjectWorld* parent,
        Type type,
        const Vec2& size,
        const Vec2& offset = Vec2{0}
    );

protected:
    Type type_ = Type::CIRCLE;

};