#pragma once

#include "core/object_affiliate.h"
#include "comm/comm_def.h"
#include <SDL3/SDL.h>

struct Texture
{
    SDL_Texture* texture = nullptr;
    SDL_FRect rect = SDL_FRect{0, 0, 0, 0};
    f32 angle = 0;
    bool is_flip = false;

    Texture() = default;
    Texture(const String& file_path);
};


class Sprite : public ObjectAffiliate
{

public:
    s32 render() override;


    Texture& get_texture() { return texture_; }
    void set_texture(const Texture& texture);

protected:
    Texture texture_;

};