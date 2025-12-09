#pragma once

#include "comm/comm_def.h"
#include "core/game.h"
#include <SDL3/SDL_events.h>
#include <format>
// #include <unordered_set>
#include <set>
#include <vector>
#include <concepts>
#include <algorithm>


class Object;

struct ObjectZOrder
{
    bool operator()(const Object* a, const Object* b) const;
};


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

    
    using ChildrenSet = std::set<Object*, ObjectZOrder>;
    
    
    
    void log_obj_tree() const noexcept;

    ChildrenSet& get_children() { return children_; }
    virtual void add_child_safe(Object* obj) { children_to_add_.push_back(obj); }
    void add_child(Object* obj)
    {
        obj->set_z_index(++max_children_z_index_);
        children_.insert(obj);
        on_add_child(obj);
    }
    virtual void on_add_child(Object* obj) { }


    virtual void remove_child_safe(Object* obj) { children_to_del_.push_back(obj); }
    void remove_child(Object* obj)
    {
        children_.erase(obj);
        on_remove_child(obj);
    }
    virtual void on_remove_child(Object* obj) { }

    bool has_child(Object* obj) const
    {
        if (std::ranges::find(children_to_del_, obj) != children_to_del_.end())
        {
            return false;
        }

        return children_.find(obj) != children_.end();
    }



    template <class T>
    requires std::derived_from<T, Object>
    bool is() const noexcept
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }

    template <class T>
    requires std::derived_from<T, Object>
    T* as() noexcept
    {
        return dynamic_cast<T*>(this);
    }


    void set_active(bool active) { is_active_ = active; }
    bool is_active() const { return is_active_; }
    void mark_for_delete() { mark_delete_ = true; }
    bool is_marked_for_delete() const { return mark_delete_; }

    s32 get_z_index() const { return z_index_; }
    void set_z_index(s32 z_index) { z_index_ = z_index; }

protected:
    Game& game_ = Game::Instance();


    ChildrenSet children_; // children objects
    std::vector<Object*> children_to_add_; // 待添加到容器内的children
    std::vector<Object*> children_to_del_; // 待删除的children

    s32 z_index_ = 1; // 渲染层级索引
    s64 max_children_z_index_ = 0; // 子节点中最大的z_index

    bool is_active_ = true;
    bool mark_delete_ = false;
};