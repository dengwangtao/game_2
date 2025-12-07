#include "effect.h"
#include "affiliate/sprite_anim.h"
#include "core/scene.h"
#include "comm/log_def.h"

s32 Effect::update(s64 now_ms, s64 delta_ms)
{
    ObjectWorld::update(now_ms, delta_ms);
    check_finish();
    return 0;
}

Effect* Effect::add_effect(Object* parent,
    const String& sprite_anim_path,
    const Vec2& pos,
    ObjectWorld* next_object,
    f32 scale
)
{
    auto effect = new Effect();
    effect->init();

    auto sprite_anim = Sprite::add_sprite<SpriteAnim>(
        effect,
        sprite_anim_path,
        Anchor::CENTER,
        scale
    );
    sprite_anim->set_loop(false);

    effect->set_sprite_anim(sprite_anim);
    effect->set_next_object(next_object);
    effect->set_pos(pos);// effect->set_world_pos(pos);

    if (parent) parent->add_child(effect);

    return effect;

}

s32 Effect::check_finish()
{
    if (sprite_anim_)
    {
        if (sprite_anim_->is_finished())
        {
            // 播放完毕，生成下一个对象
            if (next_object_)
            {
                next_object_->set_world_pos(this->get_world_pos());
                auto* scene = game_.get_curr_scene();
                if (scene)
                {
                    scene->add_child_safe(next_object_);
                }
            }

            // 标记自己删除
            this->mark_for_delete();
            LOG_DEBUG("{} finished sprite_anim_, delete this", to_string());
        }
    }
    else
    {
        LOG_DEBUG("{} has no sprite_anim_, delete this", to_string());
        this->mark_for_delete();
    }

    return 0;
}
