#include "sprite.h"
#include "resource_mgr.h"
#include "comm/log_def.h"

Texture::Texture(const String& file_path)
{
    texture = G_RESOURCE_MGR.loadResource<SDL_Texture>(file_path);

    if (!texture)
    {
        LOG_ERROR("Failed to load texture: {}", file_path);
        return;
    }

    SDL_GetTextureSize(texture, &rect.w, &rect.h);
}

s32 Sprite::render()
{
    if (texture_.texture == nullptr)
    {
        LOG_ERROR("Sprite render failed: texture is null");
        return -1;
    }

    if (! parent_)
    {
        LOG_ERROR("Sprite render failed: parent is null");
        return -2;
    }

    
    G_GAME.render_texture(
        texture_,
        parent_->get_render_pos() + offset_,
        size_
    );

    return 0;
}

void Sprite::set_texture(const Texture& texture)
{
    texture_ = texture;
    size_ = Vec2{texture.rect.w, texture.rect.h};
}
