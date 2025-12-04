#include "core/game.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "comm/log_def.h"
#include "scene_main.h"
#include "game.h"
#include "affiliate/sprite.h"


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
        now_ms_ = now_ms;
        update(now_ms, now_ms - last_frame_ns_ / NS_PER_MS);

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

s32 Game::update(s64 now_ms, s64 delta_ms)
{
    if (curr_scene_)
    {
        curr_scene_->update(now_ms, delta_ms);
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
    if (1){
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

s32 Game::draw_grid(const Vec2 &top_left, const Vec2 &bottom_right, const Vec2 &cell_size, const Color &color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);

    f32 x = top_left.x;
    while (x <= bottom_right.x)
    {
        SDL_RenderLine(renderer_, x, top_left.y, x, bottom_right.y);
        x += cell_size.x;
    }

    f32 y = top_left.y;
    while (y <= bottom_right.y)
    {
        SDL_RenderLine(renderer_, top_left.x, y, bottom_right.x, y);
        y += cell_size.y;
    }

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

    return 0;
}

s32 Game::draw_boundary(const Vec2 &top_left, const Vec2 &bottom_right, f32 thickness, const Color &color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);

    for (f32 i = 0; i < thickness; i += 1)
    {
        SDL_FRect rect = {
                top_left.x - i,
                top_left.y - i,
                bottom_right.x - top_left.x + i * 2,
                bottom_right.y - top_left.y + i * 2
        };
        SDL_RenderRect(renderer_, &rect);
    }

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

    return 0;
}

s32 Game::draw_rect(const Vec2 &pos, const Vec2 &size, const Color &color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_FRect rect = {pos.x, pos.y, size.x, size.y};
    SDL_RenderFillRect(renderer_, &rect);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    return 0;
}

s32 Game::render_texture(const Texture& texture, const Vec2& position, const Vec2& size)
{
    SDL_FRect dst_rect = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    SDL_RenderTextureRotated(
        renderer_,
        texture.texture,
        &texture.rect,
        &dst_rect,
        texture.angle,
        nullptr,
        texture.is_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
    
    return 0;
}

std::pair<Vec2, Vec2> Game::GetCameraAABB() const
{
    if (! curr_scene_)
    {
        return std::make_pair(Vec2(0, 0), Vec2(0, 0));
    }

    return {
        curr_scene_->get_camera_pos(),
        curr_scene_->get_camera_pos() + screen_size_
    };
}
