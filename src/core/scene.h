#pragma once

#include "object.h"
#include "comm/comm_def.h"
#include <vector>


class Scene : public Object
{
public:
    Scene() = default;
    ~Scene() = default;

    s32 init() override { return 0; }
    s32 handle_events(SDL_Event& event) override { return 0; }
    s32 update(s64 now_ms, s64 delta_ms) override { return 0; }
    s32 render() override { return 0; }
    s32 clean() override { return 0; }

    Vec2 world2screen(const Vec2& world_pos) const { return world_pos - camera_pos_; }
    Vec2 screen2world(const Vec2& screen_pos) const { return screen_pos + camera_pos_; }

    Vec2 get_camera_pos() const { return camera_pos_; }
    void set_camera_pos(const Vec2& pos) { camera_pos_ = pos; }

    Vec2 get_world_size() const { return world_size_; }

protected:
    Vec2 camera_pos_ = Vec2{0};
    Vec2 world_size_ = Vec2{0};
    std::vector<Object*> objects_;
};