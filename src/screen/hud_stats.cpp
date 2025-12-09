#include "hud_stats.h"
#include "affiliate/sprite.h"
#include "core/scene.h"
#include "core/actor.h"

s32 HudStats::update(s64 now_ms, s64 delta_ms)
{
    ObjectScreen::update(now_ms, delta_ms);

    // 需要实时判断对象是否还存在
    auto* scene = game_.get_curr_scene();
    if (! scene || ! target_ || ! scene->has_child(target_))
    {
        // 目标对象不存在，删除自己
        set_active(false);
        bind_stats(nullptr, nullptr, nullptr, nullptr); // 解绑数据
        // this->mark_for_delete(); // 是否需要删除？
        return 0;
    }
    
    if (hp_ && max_hp_)
    {
        f32 hp_ratio = (*hp_) / (*max_hp_);
        hp_ratio = glm::clamp(hp_ratio, 0.0f, 1.0f);

        auto sp = get_stat_sprite(HUDStatType::HP);
        if (sp)
        {
            sp->set_percent(Vec2{hp_ratio, 1.0f});
        }
    }

    if (mana_ && max_mana_)
    {
        f32 mana_ratio = (*mana_) / (*max_mana_);
        mana_ratio = glm::clamp(mana_ratio, 0.0f, 1.0f);

        auto sp = get_stat_sprite(HUDStatType::MANA);
        if (sp)
        {
            sp->set_percent(Vec2{mana_ratio, 1.0f});
        }
    }

    return 0;
}

s32 HudStats::render()
{
    ObjectScreen::render();

    constexpr Vec2 SP_ICON_SIZE = Vec2{32, 32};

    auto* sp_hp_icon = get_stat_sprite(HUDStatType::HP_ICON);
    auto* sp_hp = get_stat_sprite(HUDStatType::HP);
    auto* sp_hp_bg = get_stat_sprite(HUDStatType::HP_BG);

    if (sp_hp_icon && sp_hp && sp_hp_bg)
    {
        sp_hp_icon->set_size(SP_ICON_SIZE);


        constexpr Vec2 SP_HP_SIZE = Vec2{200, 20};
        auto pos = sp_hp_icon->get_offset()
            + sp_hp_icon->get_size() * Vec2{1.0f, 0.5f}
            - Vec2{0, SP_HP_SIZE.y * 0.5f};

            sp_hp->set_size(SP_HP_SIZE);
            sp_hp->set_offset(pos);


        sp_hp_bg->set_size(sp_hp->get_size());
        sp_hp_bg->set_offset(sp_hp->get_offset());
    }

    auto* sp_mana_icon = get_stat_sprite(HUDStatType::MANA_ICON);
    auto* sp_mana = get_stat_sprite(HUDStatType::MANA);
    auto* sp_mana_bg = get_stat_sprite(HUDStatType::MANA_BG);
    

    if (sp_mana_icon)
    {
        sp_mana_icon->set_size(SP_ICON_SIZE);
        sp_mana_icon->set_offset(Vec2{250, 0});

        constexpr Vec2 SP_MANA_SIZE = Vec2{200, 20};
        auto pos = sp_mana_icon->get_offset()
            + sp_mana_icon->get_size() * Vec2{1.0f, 0.5f}
            - Vec2{0, SP_MANA_SIZE.y * 0.5f};

        sp_mana->set_size(SP_MANA_SIZE);
        sp_mana->set_offset(pos);

        sp_mana_bg->set_size(sp_mana->get_size());
        sp_mana_bg->set_offset(sp_mana->get_offset());
    }


    return 0;
}

void HudStats::bind_stats(f32* hp, f32* max_hp, f32* mana, f32* max_mana)
{
    hp_ = hp;
    max_hp_ = max_hp;
    mana_ = mana;
    max_mana_ = max_mana;
}

HudStats* HudStats::add_hud_stats(
    Object* parent, Actor* target,
    const Vec2& screen_pos,
    const std::array<String, LEN_HUDSTAT_TYPE>& file_paths,
    f32* hp, f32* max_hp, f32* mana, f32* max_mana
)
{
    HudStats* hud_stats = new HudStats();
    hud_stats->init();

    for (size_t i = 0; i < LEN_HUDSTAT_TYPE; ++i)
    {
        auto* sprite = Sprite::add_sprite<Sprite>(
            hud_stats,
            file_paths[i],
            Anchor::TOP_LEFT,
            1.0f
        );
        hud_stats->stats_sprites_[i] = sprite;
        
        auto type = static_cast<HUDStatType>(i);

        if (type == HUDStatType::HP_ICON || type == HUDStatType::MANA_ICON)
        {
            sprite->set_z_index(99);
        }
        else if (type == HUDStatType::HP_BG || type == HUDStatType::MANA_BG)
        {
            sprite->set_z_index(97);
        }
        else
        {
            sprite->set_z_index(98);
        }

    }


    hud_stats->set_target(target);
    hud_stats->bind_stats(hp, max_hp, mana, max_mana);
    hud_stats->set_render_pos(screen_pos);

    if (parent) parent->add_child(hud_stats);

    return hud_stats;

}
