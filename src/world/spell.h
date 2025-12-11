#pragma once


#include "core/object_world.h"

class SpriteAnim;
class Actor;

class Spell : public ObjectWorld
{

public:
    s32 init() override;
    s32 update(s64 now_ms, s64 delta_ms) override;


    void set_sprite(SpriteAnim* sprite) { sprite_ = sprite; }
    SpriteAnim* get_sprite() const { return sprite_; }

    Actor* get_spawner() const { return spawner_; }
    void set_spawner(Actor* actor) { spawner_ = actor; }

    static Spell* add_spell(
        Object* parent,
        Actor* spawner,
        const String& file_path,
        const Vec2& pos,
        f32 damage = 30.0f,
        f32 scale = 1.0f,
        Anchor anchor = Anchor::CENTER
    );

private:
    s32 attack();

private:

    SpriteAnim* sprite_ = nullptr;
    f32 damage_ = 30.0f;
    Actor* spawner_ = nullptr;
};