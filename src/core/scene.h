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
    s32 update(s64 now_ms) override { return 0; }
    s32 render() override { return 0; }
    s32 clean() override { return 0; }

private:
    Vec2 camera_pos_ = Vec2{0};
    std::vector<Object*> objects_;
};