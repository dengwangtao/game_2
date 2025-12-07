
#include "object.h"
#include "comm/log_def.h"
#include "world/effect.h"
#include <functional>

template<class F>
void for_children(std::unordered_set<Object*>& children,
                  std::vector<Object*>& to_add,
                  F&& fn)
{
    for (auto* ptr : to_add)
    {
        children.insert(ptr);
    }
    to_add.clear();

    for (auto it = children.begin(); it != children.end(); )
    {
        auto* c = *it;
        if (c->is_marked_for_delete())
        {
            it = children.erase(it);
            c->clean();
            delete c;
        }
        else
        {
            fn(c);
            ++it;
        }
    }
}

s32 Object::handle_events(SDL_Event& event)
{
    for_children(children_, children_to_add_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->handle_events(event);
    });

    return 0;
}

s32 Object::update(s64 now_ms, s64 delta_ms)
{
    for_children(children_, children_to_add_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->update(now_ms, delta_ms);
    });
    return 0;
}

s32 Object::render()
{
    for_children(children_, children_to_add_, [&](Object* child)
    {
        if (! child->is_active()) return;
        child->render();
    });
    return 0;
}

s32 Object::clean()
{
    LOG_DEBUG("Cleaning Object and its children: {}", to_string());

    for_children(children_, children_to_add_, [&](Object* child)
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
