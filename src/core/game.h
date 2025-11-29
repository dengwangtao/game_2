#pragma once

#include <string>

#include "comm/singleton.h"
#include "comm/comm_def.h"


struct SDL_Window;
struct SDL_Renderer;

class Scene;

class Game : public Singleton<Game>
{
public:
    s32 init(const String& title, int width, int height);
    s32 run();
    s32 handle_events();
    s32 update(s64 now_ms);
    s32 render();
    s32 clean();


    // 背景网格
    s32 draw_grid(const Vec2& top_left, const Vec2& bottom_right, const Vec2& cell_size, const Color& color);
    s32 draw_boundary(const Vec2& top_left, const Vec2& bottom_right, f32 thickness, const Color& color);

    Vec2 get_screen_size() const { return screen_size_; }

private:
    Vec2 screen_size_ = Vec2{0};
    bool is_running_ = true;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    
    const u64 fps_ = 60;
    const u64 frame_duration_ = NS_PER_SEC / fps_; // in ns
    f64 dt_ = 0;
    u64 last_frame_ns_ = 0;


    Scene* curr_scene_ = nullptr;
};