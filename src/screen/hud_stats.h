#pragma once


#include "core/object_screen.h"
#include <array>

class Sprite;
class Actor;


enum class HUDStatType
{
    HP, HP_BG, HP_ICON,
    MANA, MANA_BG, MANA_ICON,

    LEN_STAT_TYPE
};
constexpr size_t LEN_HUDSTAT_TYPE = static_cast<size_t>(HUDStatType::LEN_STAT_TYPE);



class HudStats : public ObjectScreen
{

public:
    
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;
    
    void bind_stats(f32* hp, f32* max_hp, f32* mana, f32* max_mana);


    void set_target(Actor* target) { target_ = target; }
    Actor* get_target() const { return target_; }

    Sprite* get_stat_sprite(HUDStatType type) const
    {
        if (static_cast<size_t>(type) >= LEN_HUDSTAT_TYPE)
        {
            return nullptr;
        }
        return stats_sprites_[static_cast<size_t>(type)];
    }
    void set_stat_sprite(HUDStatType type, Sprite* sprite)
    {
        if (static_cast<size_t>(type) >= LEN_HUDSTAT_TYPE)
        {
            return;
        }
        stats_sprites_[static_cast<size_t>(type)] = sprite;
    }


    static HudStats* add_hud_stats(
        Object* parent,
        Actor* target,
        const std::array<String, LEN_HUDSTAT_TYPE>& file_paths,
        f32* hp, f32* max_hp, f32* mana, f32* max_mana
    );

private:
    Actor* target_ = nullptr;

    std::array<Sprite*, LEN_HUDSTAT_TYPE> stats_sprites_;

    f32* hp_ = nullptr;
    f32* max_hp_ = nullptr;
    f32* mana_ = nullptr;
    f32* max_mana_ = nullptr;
};