#include "collider.h"
#include "comm/log_def.h"
#include "core/object_world.h"

s32 Collider::render()
{
    ObjectAffiliate::render();


#ifdef DWT_DEBUG
    // 绘制碰撞盒
do
{
    if (! parent_) break;        

    auto [min, max, valid] = get_aabb(true);
    if(! valid) break;

    G_GAME.draw_boundary(
        min,
        max,
        1.0f,
        Color(255, 0, 0, 128)
    );

    // 绘制detail shape
    switch (type_)
    {
        case Type::CIRCLE:
        {
            f32 radius = size_.x / 2.0f;
            Vec2 center = parent_->get_render_pos() + offset_ + radius;
            G_GAME.draw_circle_outline(
                center,
                radius,
                Color(0, 255, 0, 128)
            );
            break;
        }
        case Type::RECT:
        {
            Vec2 pos = parent_->get_render_pos() + offset_;
            G_GAME.draw_rect(
                pos,
                size_,
                Color(0, 255, 0, 128)
            );
            break;
        }
        default:
            break;
    }

} while (0);

#endif

    return 0;
}

bool Collider::is_colliding(const Collider& other) const {
    if (!parent_ || !other.parent_) {
        LOG_WARN(
            "Cannot perform collision detection: one of the colliders has no "
            "parent");
        return false;
    }

    if (!is_aabb_colliding(other))
    {
        return false;
    }

    // 进一步的精确碰撞检测可以在这里实现

    if (get_type() == Type::CIRCLE && other.get_type() == Type::CIRCLE) {
        // 圆形碰撞检测
        f32 radius1 = size_.x / 2.0f;
        f32 radius2 = other.size_.x / 2.0f;

        auto pos1 = parent_->pos() + offset_ + radius1;
        auto pos2 = other.parent_->pos() + other.offset_ + radius2;

        f32 dist_sq = glm::length(pos1 - pos2);

        return dist_sq <= radius1 + radius2;
    }

    // TODO: 添加其他类型的碰撞检测（如矩形与矩形，圆形与矩形等）

    return false;
}

bool Collider::is_aabb_colliding(const Collider& other) const
{
    auto [min1, max1, valid1] = get_aabb();
    auto [min2, max2, valid2] = other.get_aabb();
    if (!valid1 || !valid2)
    {
        LOG_WARN("Cannot perform AABB collision detection: invalid AABB");
        return false;
    }

    // AABB碰撞检测
    if (max1.x < min2.x || min1.x > max2.x) return false; // 在x轴上没有重叠
    if (max1.y < min2.y || min1.y > max2.y) return false; // 在y轴上没有重叠

    return true; // 碰撞
}

std::tuple<Vec2, Vec2, bool> Collider::get_aabb(bool render) const
{
    if (!parent_)
    {
        LOG_WARN("Collider has no parent, cannot get AABB");
        return {{}, {}, false};
    }

    auto p = render ? parent_->get_render_pos() : parent_->pos();

    switch (type_)
    {
        case Type::CIRCLE:
        {
            f32 radius = size_.x / 2.0f; // size_.x 为直径
            Vec2 center = p + offset_ + radius;
            Vec2 min = center - Vec2{radius};
            Vec2 max = center + Vec2{radius};
            return {min, max, true};
        }
        case Type::RECT:
        {
            Vec2 min = p + offset_;
            Vec2 max = min + size_;
            return {min, max, true};
        }
        default:
            break;
    }

    return {{}, {}, false};
}

Collider* Collider::add_collider(ObjectWorld* parent, Type type, const Vec2& size,
                           Anchor anchor)
{
    auto collider = new Collider();
    collider->init();
    collider->set_parent(parent);
    collider->set_type(type);
    collider->set_size(size);
    collider->set_offset_by_anchor(anchor);

    parent->set_collider(collider);
    parent->add_child(collider);

    return collider;

}
