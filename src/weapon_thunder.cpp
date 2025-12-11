#include "weapon_thunder.h"

#include "core/scene.h"
#include "comm/log_def.h"
#include "world/spell.h"


s32 WeaponThunder::handle_events(SDL_Event& event)
{
    bool is_attack =
            (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) 
            ||
            (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_Q);
    
    bool enhance = (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_Q);

    if (is_attack)
    {
        
        auto* scene = game_.get_curr_scene();
        if (!scene)
        {
            LOG_ERROR("WeaponThunder::handle_events failed: no current scene");
            return -1;
        }

        if (can_attack(enhance))
        {
            auto world_pos = scene->screen2world(game_.get_mouse_pos());

            LOG_DEBUG("WeaponThunder attack at world({}, {})",
                world_pos.x, world_pos.y
            );

            auto* spell = Spell::add_spell(
                nullptr,
                owner_,
                "../assets/effect/Thunderstrike w blur.png",
                world_pos,
                40.0f,
                3.0f,
                Anchor::BOTTOM_CENTER
            );


            attack(world_pos, spell, enhance);
            LOG_INFO("attack in [{},{}], enhance:{}", world_pos.x, world_pos.y, enhance);
        }

        
    }

    return 0;
}