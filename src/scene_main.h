#pragma once


#include "core/scene.h"
#include "spawner.h"

class Player;

class SceneMain : public Scene
{
public:
    s32 init() override;
    s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    s32 clean() override;

    std::string to_string() const override
    { return "SceneMain:" + std::format("{:x}", (uintptr_t)this); }

private:
    s32 render_background();

    
    Player* player_ = nullptr;
    Spawner* spawner_ = nullptr;
};