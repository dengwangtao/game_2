#include <iostream>
#include "SDL3/SDL.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "core/game.h"
#include "comm/log_def.h"

int main(int, char**)
{
    logdef::init_log();

    auto& g = Game::Instance();
    s32 ret = g.init("ghost escape", 1280, 720);
    if (ret)
    {
        LOG_ERROR("Game initialization failed with error code {}", ret);
        return ret;
    }

    ret = g.run();
    if (ret)
    {
        LOG_ERROR("Game run failed with error code {}", ret);
        return ret;
    }

    g.clean();

    return 0;
}