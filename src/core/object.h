#pragma once

#include "comm/comm_def.h"
#include "core/game.h"
#include <SDL3/SDL_events.h>
#include <format>
#include <unordered_set>
#include <vector>
#include <concepts>

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

    virtual std::string to_string() const
    { return "Object:" + std::format("{:x}", (uintptr_t)this); }

    void log_obj_tree() const noexcept;

    std::unordered_set<Object*>& get_children() { return children_; }
    virtual void add_child_safe(Object* obj) { children_to_add_.push_back(obj); }
    virtual void add_child(Object* obj) { children_.insert(obj); }
    virtual void remove_child(Object* obj) { children_.erase(obj); }


    template <class T>
    requires std::derived_from<T, Object>
    bool is() const noexcept
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }


    void set_active(bool active) { is_active_ = active; }
    bool is_active() const { return is_active_; }
    void mark_for_delete() { mark_delete_ = true; }
    bool is_marked_for_delete() const { return mark_delete_; }

protected:
    Game& game_ = Game::Instance();

    std::unordered_set<Object*> children_; // children objects
    std::vector<Object*> children_to_add_; // 待添加到容器内的children

    bool is_active_ = true;
    bool mark_delete_ = false;
};