#include "spell.h"
#include "affiliate/sprite_anim.h"
#include "affiliate/collider.h"
#include "comm/log_def.h"
#include "core/scene.h"
#include "enemy.h"

s32 Spell::init()
{
    ObjectWorld::init();

    return 0;
}

s32 Spell::update(s64 now_ms, s64 delta_ms)
{
    ObjectWorld::update(now_ms, delta_ms);

    if (sprite_)
    {
        if (sprite_->is_finished())
        {
            mark_for_delete();
        }
        else
        {
            attack();
        }
    }

    

    return 0;
}

void Spell::scale_size(f32 factor)
{
    if (sprite_)
    {
        sprite_->scale(factor);
    }
    if (collider_)
    {
        collider_->scale(factor);
    }
}

void Spell::scale_damage(f32 factor)
{
    damage_ *= factor;
}

Spell* Spell::add_spell(Object* parent, Actor* spawner, const String& file_path,
                        const Vec2& pos, f32 damage, f32 scale, Anchor anchor)
{
    Spell* spell = new Spell();
    spell->init();

    spell->set_world_pos(pos);
    spell->damage_ = damage;
    spell->set_spawner(spawner);

    auto sprite = Sprite::add_sprite<SpriteAnim>(spell, file_path, anchor, scale);
    sprite->set_loop(false);
    spell->set_sprite(sprite);

    // 添加碰撞器
    auto collider = Collider::add_collider(
        spell, Collider::Type::CIRCLE, Vec2{sprite->get_size().x * 0.5f},  // 直径
        Anchor::CENTER);
    
    spell->set_collider(collider);

    if (parent) parent->add_child(spell);

    return spell;
}

s32 Spell::attack()
{
    auto scene = game_.get_curr_scene();
    if (! scene)
    {
        LOG_ERROR("Spell::attack failed: no current scene");
        return -1;
    }

    if (! collider_)
    {
        LOG_ERROR("Spell::attack failed: no collider");
        return -2;
    }

    auto& world_obj = scene->get_world_children();
    for (auto* obj : world_obj)
    {
        auto* obj_collider = obj->get_collider();
        if (!obj_collider)
        {
            continue;
        }

        if (!obj->is<Enemy>())
        {
            continue;
        }

        auto* enemy = obj->as<Enemy>();
        if (! enemy)
        {
            LOG_ERROR("Spell::attack failed: as<> to Enemy failed");
            continue;
        }

        if (collider_->is_colliding(*obj_collider))
        {
            LOG_TRACE("Spell hit an object!");

            enemy->take_damage(damage_, spawner_);
        }
    }

  return 0;
}
