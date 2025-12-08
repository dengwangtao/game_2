#pragma once


#include "raw/weapon.h"



class WeaponThunder : public Weapon
{

public:

    s32 handle_events(SDL_Event& event) override;

};