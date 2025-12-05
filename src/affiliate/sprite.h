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

    void scale(f32 factor)
    {
        size_ *= factor;
    }

    Texture& get_texture() { return texture_; }
    virtual void set_texture(const Texture& texture);

    bool is_finished() const { return is_finished_; }
    void set_finished(bool finished) { is_finished_ = finished; }


    template<std::derived_from<Sprite> T>
    static T* add_sprite(
        ObjectScreen* parent,
        const String& file_path,
        f32 scale_factor = 1.0f,
        bool is_active = true,
        T* = nullptr
    );

protected:
    Texture texture_;
    bool is_finished_ = false;

};

template <std::derived_from<Sprite> T>
inline T* Sprite::add_sprite(ObjectScreen* parent, const String& file_path,
                              f32 scale_factor, bool is_active,
                              T*)
{
    auto sprite = new T();
    sprite->init();
    sprite->set_texture(
        Texture(file_path)
    );
    sprite->scale(scale_factor);
    sprite->set_parent(parent);
    sprite->set_active(is_active);

    parent->add_child(sprite);

    return sprite;
}
