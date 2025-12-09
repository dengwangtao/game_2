#pragma once




#include "core/actor.h"

#define PROP_DEF(type, name, def) \
protected: type name##_ = def; \
    public: \
     type get_##name() const { return name##_; } \
    void set_##name(type value) { name##_ = value; }\
    type* get_ptr_##name() { return &name##_; }

#define PROP(type, name) PROP_DEF(type, name, type{})

class Stats : public Actor
{


public:
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;



    bool can_use_mana(f32 cost);
    s32 use_mana(f32 cost);
    s32 regen_mana(s64 delta_ms); // 回蓝
    s32 take_damage(f32 damage);  // 受伤
    s32 update_invincibility(s64 delta_ms); // 更新无敌状态


    static Stats* add_stats(
        Actor* parent,
        f32 hp = 100.0f,
        f32 max_hp = 100.0f,
        f32 mana = 50.0f,
        f32 max_mana = 50.0f,
        f32 attack = 10.0f,
        f32 mana_regen = 5.0f // 每秒恢复的法力值
    );

protected:
    PROP_DEF(Actor*, parent, nullptr);

    PROP_DEF(f32, hp, 100.0f);
    PROP_DEF(f32, max_hp, 100.0f);

    PROP_DEF(f32, mana, 50.0f);
    PROP_DEF(f32, max_mana, 50.0f);

    PROP_DEF(f32, attack, 10.0f);
    PROP_DEF(f32, mana_regen, 5.0f); // 每秒恢复的法力值

    PROP_DEF(bool, is_invincible, false); // 是否无敌
    PROP_DEF(s64, invincible_timer, 0); // 无敌计时器

    PROP_DEF(bool, is_alive, true);

};