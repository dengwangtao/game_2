
#include "object.h"

s32 Object::handle_events(SDL_Event& event)
{
    for (auto* child : children_)
    {
        child->handle_events(event);
    }

    return 0;
}

s32 Object::update(s64 now_ms, s64 delta_ms)
{
    for (auto* child : children_)
    {
        child->update(now_ms, delta_ms);
    }
    return 0;
}

s32 Object::render()
{
    for (auto* child : children_)
    {
        child->render();
    }
    return 0;
}

s32 Object::clean()
{
    for (auto* child : children_)
    {
        child->clean();
        delete child;
    }
    children_.clear();

    return 0;
}
