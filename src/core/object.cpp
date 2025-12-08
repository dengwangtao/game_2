
#include "object.h"
#include "comm/log_def.h"
#include "world/effect.h"
#include <functional>

template<class F>
void for_children(
    Object* this_ptr,
    std::unordered_set<Object*>& children,
    std::vector<Object*>& to_add,
    std::vector<Object*>& to_del,
    F&& fn)
{
    // 先处理待删除的children
    for (auto* ptr : to_del)
    {
        this_ptr->remove_child(ptr);
    }
    to_del.clear();

    for (auto* ptr : to_add)
    {
        this_ptr->add_child(ptr);
    }
    to_add.clear();

    for (auto it = children.begin(); it != children.end(); )
    {
        auto* c = *it;
        if (c->is_marked_for_delete())
        {
            this_ptr->remove_child_safe(c);
        }
        else
        {
            fn(c);
        }
        ++it;
    }
}

s32 Object::handle_events(SDL_Event& event)
{
    for_children(this, children_, children_to_add_, children_to_del_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->handle_events(event);
    });

    return 0;
}

s32 Object::update(s64 now_ms, s64 delta_ms)
{
    for_children(this, children_, children_to_add_, children_to_del_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->update(now_ms, delta_ms);
    });
    return 0;
}

s32 Object::render()
{
    for_children(this, children_, children_to_add_, children_to_del_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->render();
    });
    return 0;
}

s32 Object::clean()
{
    LOG_DEBUG("Cleaning Object and its children: {}", to_string());

    for_children(this, children_, children_to_add_, children_to_del_, [&](Object* child)
    {
        child->clean();
        delete child;
    });
    children_.clear();

    return 0;
}

void Object::log_obj_tree() const noexcept
{
    std::function<void(const Object*, s32)> dfs = [&](const Object* obj, s32 depth)
    {
        if (! obj) return;
        LOG_DEBUG("{} {}", std::string(depth, '\t'), obj->to_string());
        for (auto* child : obj->children_)
        {
            dfs(child, depth + 1);
        }
    };

    dfs(this, 0);
}
