#include "object_affiliate.h"

void ObjectAffiliate::set_size(const Vec2& size)
{
    size_ = size;

    // 更新offset以匹配当前锚点
    set_offset_by_anchor(anchor_);
}

void ObjectAffiliate::set_offset_by_anchor(Anchor anchor)
{
    anchor_ = anchor;

    if (!parent_)
    {
        return;
    }

    switch (anchor_)
    {
        case Anchor::TOP_LEFT:
            offset_ = Vec2{0, 0};
            break;
        case Anchor::TOP_CENTER:
            offset_ = Vec2{-(size_.x) / 2.0f, 0};
            break;
        case Anchor::TOP_RIGHT:
            offset_ = Vec2{-size_.x, 0};
            break;
        case Anchor::CENTER_LEFT:
            offset_ = Vec2{0, -(size_.y) / 2.0f};
            break;
        case Anchor::CENTER:
            offset_ = Vec2{-(size_.x) / 2.0f, -(size_.y) / 2.0f};
            break;
        case Anchor::CENTER_RIGHT:
            offset_ = Vec2{-size_.x, -(size_.y) / 2.0f};
            break;
        case Anchor::BOTTOM_LEFT:
            offset_ = Vec2{0, -size_.y};
            break;
        case Anchor::BOTTOM_CENTER:
            offset_ = Vec2{-(size_.x) / 2.0f, -size_.y};
            break;
        case Anchor::BOTTOM_RIGHT:
            offset_ = Vec2{-size_.x, -size_.y};
            break;
        default:
            break;
    }
}