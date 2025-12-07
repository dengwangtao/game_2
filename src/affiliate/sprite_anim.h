#pragma once


#include "affiliate/sprite.h"
#include "comm/comm_def.h"

class SpriteAnim: public Sprite
{

public:
    s32 update(s64 now_ms, s64 delta_ms) override;
    std::string to_string() const override
    { return "SpriteAnim:" + std::format("{:x}", (uintptr_t)this); }

    void set_texture(const Texture& texture) override;

    s32 copy_state(const SpriteAnim& other);

    s32 get_current_frame() const { return current_frame_; }
    void set_current_frame(s32 frame) { current_frame_ = frame; }
    s32 get_total_frames() const { return total_frames_; }
    void set_total_frames(s32 total_frames) { total_frames_ = total_frames; }
    s32 get_fps() const { return fps_; }
    void set_fps(int fps) { fps_ = fps; }
    f32 get_frame_timer() const { return frame_timer_; }
    void set_frame_timer(f32 timer) { frame_timer_ = timer; }
    bool is_loop() const { return is_loop_; }
    void set_loop(bool loop) { is_loop_ = loop; }

protected:
    s32 current_frame_ = 0;
    s32 total_frames_ = 0;
    int fps_ = 10;
    s64 frame_timer_ = 0;
    bool is_loop_ = true;


};
