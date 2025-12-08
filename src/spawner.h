#pragma once

#include "core/object.h"
#include "comm/comm_def.h"

class Player;

class Spawner : public Object
{

public:
    s32 update(s64 now_ms, s64 delta_ms) override;

    s32 get_num() const { return num_; }
    void set_num(s32 num) { num_ = num; }

    s64 get_timer() const { return timer_; }
    void set_timer(s64 timer) { timer_ = timer; }

    s64 get_interval() const { return interval_; }
    void set_interval(s64 interval) { interval_ = interval; }

    Player* get_target_player() const { return target_player_; }
    void set_target_player(Player* player) { target_player_ = player; }

protected:
    s32 num_ = 4;
    s64 timer_ = 0;
    s64 interval_ = 5000; // ms
    Player* target_player_ = nullptr;
};