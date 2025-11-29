#pragma once


#include "core/scene.h"

class SceneMain : public Scene
{
public:
    s32 init() override;
    s32 handle_events(SDL_Event& event) override;
    s32 update(s64 now_ms) override;
    s32 render() override;
    s32 clean() override;
};