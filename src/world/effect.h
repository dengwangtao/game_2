#pragma once



#include "core/object_world.h"
#include "comm/comm_def.h"


class SpriteAnim;

class Effect : public ObjectWorld
{
public:
    s32 update(s64 now_ms, s64 delta_ms) override;

    std::string to_string() const override
    { return "Effect:" + std::format("{:x}", (uintptr_t)this); }

    SpriteAnim* get_sprite_anim() const { return sprite_anim_; }
    void set_sprite_anim(SpriteAnim* sprite_anim) { sprite_anim_ = sprite_anim; }
    ObjectWorld* get_next_object() const { return next_object_; }
    void set_next_object(ObjectWorld* next_object) { next_object_ = next_object; }



    static Effect* add_effect(
        Object* parent,
        const String& sprite_anim_path,  
        const Vec2& pos,
        ObjectWorld* next_object = nullptr,
        f32 scale = 1.0f
    );

private:
    s32 check_finish();


protected:
    SpriteAnim* sprite_anim_ = nullptr;
    ObjectWorld* next_object_ = nullptr; // 播放完毕后要生成的对象
};