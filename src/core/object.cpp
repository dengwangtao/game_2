
#include "object.h"


#define FOR_CHILDREN_BEGIN \
    for (auto* child : children_) { \
        if (!child->is_active()) continue;

#define FOR_CHILDREN_END \
    }


s32 Object::handle_events(SDL_Event& event)
{
    FOR_CHILDREN_BEGIN
        child->handle_events(event);
    FOR_CHILDREN_END

    return 0;
}

s32 Object::update(s64 now_ms, s64 delta_ms)
{
    FOR_CHILDREN_BEGIN
        child->update(now_ms, delta_ms);
    FOR_CHILDREN_END
    return 0;
}

s32 Object::render()
{
    FOR_CHILDREN_BEGIN
        child->render();
    FOR_CHILDREN_END
    return 0;
}

s32 Object::clean()
{
    FOR_CHILDREN_BEGIN
        child->clean();
        delete child;
    FOR_CHILDREN_END
    children_.clear();

    return 0;
}
