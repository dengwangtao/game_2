#pragma once

#include "comm/comm_def.h"
#include "core/game.h"
#include <SDL3/SDL_events.h>

#include <unordered_set>

class Object
{

public:
    Object() = default;
    virtual ~Object() = default;
    
    virtual s32 init() { return 0; }
    virtual s32 handle_events(SDL_Event& event);
    virtual s32 update(s64 now_ms, s64 delta_ms);
    virtual s32 render();
    virtual s32 clean();

    std::unordered_set<Object*>& get_children() { return children_; }
    virtual void add_child(Object* obj) { children_.insert(obj); }
    virtual void remove_child(Object* obj) { children_.erase(obj); }

protected:
    Game& game_ = Game::Instance();

    std::unordered_set<Object*> children_; // children objects

};