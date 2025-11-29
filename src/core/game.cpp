#include "core/game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "comm/log_def.h"
#include "scene_main.h"


s32 Game::init(const String &title, int width, int height)
{
    screen_size_ = Vec2{width, height};

    if (! SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        LOG_ERROR("SDL initialized failed");
        return -1;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        LOG_ERROR("SDL_mixer initialized failed");
        return -2;
    }

    if (! Mix_OpenAudio(0, nullptr))
    {
        LOG_ERROR("SDL_mixer open audio failed");
        return -3;
    }

    Mix_AllocateChannels(16); // 分配16个通道

    // 设置音量为25%
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4); // 0~128
    Mix_Volume(-1, MIX_MAX_VOLUME / 4); // 25%


    
    if (! TTF_Init())
    {
        LOG_ERROR("SDL_ttf initialized failed");
        return -4;
    }


    // 创建窗口和渲染器
    s32 flags = SDL_WINDOW_RESIZABLE; // | ... // 其他SDL_WINDOW_xxx参数
    bool ret = SDL_CreateWindowAndRenderer(title.c_str(), width, height, flags, &window_, &renderer_);
    if (!ret || ! window_ || !renderer_)
    {
        LOG_ERROR("SDL_CreateWindowAndRenderer failed");
        return -5;
    }

    // 设置窗口逻辑分辨率
    SDL_SetRenderLogicalPresentation(renderer_, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    // 创建场景
    curr_scene_ = new SceneMain();
    curr_scene_->init();

    LOG_INFO("Game [{}] initialized", title);

    return 0;
}

s32 Game::run()
{
    while (is_running_)
    {
        // 处理事件
        handle_events();

        // 更新
        s64 now_ms = SDL_GetTicks();
        update(now_ms);

        // 渲染
        render();

        // 帧率控制
        auto now_ns = SDL_GetTicksNS();
        auto elapsed_ns = now_ns - last_frame_ns_;
        if (elapsed_ns < frame_duration_)
        {
            SDL_DelayNS(frame_duration_ - elapsed_ns);
            dt_ = static_cast<f64>(frame_duration_) / NS_PER_MS;
        }
        else
        {
            dt_ = static_cast<f64>(elapsed_ns) / NS_PER_MS;
        }

        LOG_TRACE("frame time: {} ms fps: {}", dt_, 1000.0 / dt_);
        
        last_frame_ns_ = now_ns;

    }

    return 0;
}

s32 Game::handle_events()
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            is_running_ = false;
        }

        if (curr_scene_)
        {
            curr_scene_->handle_events(event);
        }
    }
    return 0;
}

s32 Game::update(s64 now_ms)
{
    if (curr_scene_)
    {
        curr_scene_->update(now_ms);
    }
    return 0;
}

s32 Game::render()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    // 清屏
    SDL_RenderClear(renderer_);
    if (curr_scene_)
    {
        curr_scene_->render();
    }

    // TODO: 测试代码, 后面需要删除
    {
        auto mouse_pos = Vec2(0, 0);

        auto state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        {
            auto getbit = [](int c, int i) ->u8 { return (c & (1 << i)) ? 255 : 128; };

            SDL_Color color = {getbit(state, 0), getbit(state, 1), getbit(state, 2), 255}; // 默认灰色
            SDL_FRect rect = {mouse_pos.x - 20, mouse_pos.y - 20, 40, 40};
            SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer_, &rect);
        }
    }

    SDL_RenderPresent(renderer_);
    return 0;
}

s32 Game::clean()
{
    if (curr_scene_)
    {
        curr_scene_->clean();
        delete curr_scene_;
        curr_scene_ = nullptr;
    }

    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    LOG_INFO("Game cleaned");

    return 0;
}
