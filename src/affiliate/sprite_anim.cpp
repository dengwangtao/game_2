#include "sprite_anim.h"

s32 SpriteAnim::update(s64 now_ms, s64 delta_ms)
{
    frame_timer_ += delta_ms;

    if (frame_timer_ >= (1000.0f / fps_))
    {
        current_frame_ = (current_frame_ + 1) % total_frames_;
        frame_timer_ = 0;

        // 更新纹理的rect以显示当前帧
        if (texture_.texture)
        {
            texture_.rect.x = current_frame_ * texture_.rect.w;
        }
    }

    return 0;
}

void SpriteAnim::set_texture(const Texture& texture)
{
    Sprite::set_texture(texture);

    total_frames_ = texture_.rect.w / texture_.rect.h;

    texture_.rect.w = texture_.rect.h;

    size_ = Vec2{texture_.rect.w, texture_.rect.h};
}

s32 SpriteAnim::copy_state(const SpriteAnim& other)
{
    current_frame_ = other.current_frame_;
    if (current_frame_ >= total_frames_)
    {
        current_frame_ = 0;
    }
    frame_timer_ = other.frame_timer_;

    return 0;
}
