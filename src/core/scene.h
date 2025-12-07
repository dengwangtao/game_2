#pragma once

#include "object.h"
#include "comm/comm_def.h"
#include <vector>
#include <unordered_set>
#include <array>


class ObjectWorld;
class ObjectScreen;

class Scene : public Object
{
public:
    Scene() = default;
    ~Scene() = default;

    s32 init() override;
    s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    s32 clean() override;
    std::string to_string() const override
    { return "Scene:" + std::format("{:x}", (uintptr_t)this); }

    Vec2 world2screen(const Vec2& world_pos) const { return world_pos - camera_pos_; }
    Vec2 screen2world(const Vec2& screen_pos) const { return screen_pos + camera_pos_; }

    Vec2 get_camera_pos() const { return camera_pos_; }
    void set_camera_pos(const Vec2& pos) { camera_pos_ = pos; }

    Vec2 get_world_size() const { return world_size_; }


    void add_child(Object* obj) override;
    void remove_child(Object* obj) override;

protected:
    Vec2 camera_pos_ = Vec2{0};
    Vec2 world_size_ = Vec2{0};
    
    std::unordered_set<ObjectWorld*> childern_world_;
    std::unordered_set<ObjectScreen*> childern_screen_;
};