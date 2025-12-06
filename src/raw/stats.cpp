#include "stats.h"
#include "comm/log_def.h"


s32 Stats::update(s64 now_ms, s64 delta_ms)
{
    Actor::update(now_ms, delta_ms);

    regen_mana(delta_ms);

    return 0;
}

s32 Stats::render()
{
    Actor::render();


#ifdef DWT_DEBUG
    do
    {
        if (get_parent() == nullptr)
        {
            break;
        }

        auto render_pos = get_parent()->get_render_pos();

        // 文字形式输出debug信息
        std::string stats_text = 
            "HP: " + std::to_string((s32)hp_) + "/" + std::to_string((s32)max_hp_) +
            "  Mana: " + std::to_string((s32)mana_) + "/" + std::to_string((s32)max_mana_) +
            "  Attack: " + std::to_string((s32)attack_) +
            "  Mana Regen: " + std::to_string((s32)mana_regen_) + "/s";

        G_GAME.draw_debug_text(
            render_pos + Vec2{-20.0f, 30.0f},
            stats_text
        );
            
        
    } while (0);
    

    

#endif


    return 0;
}

bool Stats::can_use_mana(f32 cost)
{
    if (mana_ < cost)
    {
        return false;
    }

    return true;
}

s32 Stats::use_mana(f32 cost)
{
    if (!can_use_mana(cost))
    {
        return -1;
    }

    mana_ -= cost;
    if (mana_ < 0)
    {
        mana_ = 0;
    }

    return 0;
}

s32 Stats::regen_mana(s64 delta_ms)
{
    f32 mana_to_regen = (mana_regen_ * delta_ms) / 1000.0f;
    mana_ += mana_to_regen;
    if (mana_ > max_mana_)
    {
        mana_ = max_mana_;
    }

    return 0;
}

s32 Stats::take_damage(f32 damage)
{
    hp_ -= damage;
    if (hp_ < 0)
    {
        hp_ = 0;
        is_alive_ = false;
    }

    return 0;
}

Stats* Stats::add_stats(Actor* parent, f32 hp, f32 max_hp, f32 mana,
                        f32 max_mana, f32 attack, f32 mana_regen)
{
    auto stats = new Stats();
    stats->init();
    stats->set_parent(parent);
    stats->set_hp(hp);
    stats->set_max_hp(max_hp);
    stats->set_mana(mana);
    stats->set_max_mana(max_mana);
    stats->set_attack(attack);
    stats->set_mana_regen(mana_regen);

    parent->set_stats(stats);
    parent->add_child(stats);

    return stats;
}
