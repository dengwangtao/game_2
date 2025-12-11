#pragma once

#include "core/object.h"
#include "core/actor.h"

// class Actor;
class Spell;

class Weapon : public Object
{

public:
    s32 update(s64 now_ms, s64 delta_ms) override;


    bool can_attack(bool enhance=false) const; // enhance为强化技能
    s32 attack(const Vec2& world_pos, Spell* spell, bool enhance=false);


    s64 get_cooldown_timer() const { return cooldown_timer_; }
    void set_cooldown_timer(s64 timer) { cooldown_timer_ = timer; }
    s64 get_cooldown_duration() const { return cooldown_duration_; }
    void set_cooldown_duration(s64 duration) { cooldown_duration_ = duration; }
    s32 get_mana_cost() const { return mana_cost_; }
    void set_mana_cost(s32 cost) { mana_cost_ = cost; }
    Actor* get_owner() const { return owner_; }
    void set_owner(Actor* owner) { owner_ = owner; }

    template <class T>
    requires std::derived_from<T, Weapon>
    static Weapon* add_weapon(
        Actor* owner,
        s64 cooldown_duration = 1000,
        s32 mana_cost = 10
    );

protected:
    s64 cooldown_timer_ = 0; // 冷却计时器
    s64 cooldown_duration_ = 500; // 冷却持续时间，单位毫秒

    s32 mana_cost_ = 10; // 魔法消耗
    f32 enhance_cost_factor_ = 1.3f;
    Actor* owner_ = nullptr;
};

template <class T>
requires std::derived_from<T, Weapon>
inline Weapon* Weapon::add_weapon(Actor* owner, s64 cooldown_duration,
                                  s32 mana_cost)
{
    T* weapon = new T();
    weapon->init();
    weapon->set_owner(owner);
    weapon->set_cooldown_duration(cooldown_duration);
    weapon->set_cooldown_timer(cooldown_duration); // 刚创建时就可以使用
    weapon->set_mana_cost(mana_cost);

    if (owner)
    {
        owner->add_child(weapon);
    }

    return weapon;
}
