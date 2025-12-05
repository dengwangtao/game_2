
#include "object.h"
#include "comm/log_def.h"

#define FOR_CHILDREN_BEGIN \
    for (auto it = children_.begin(); it != children_.end();) \
    { \
        auto* child = *it; \
        if (child->is_marked_for_delete()) \
        { \
            it = children_.erase(it); \
            child->clean(); \
            delete child; \
            continue; \
        } \
        if (!child->is_active()) { \
            ++ it; \
            continue; \
        } \

#define FOR_CHILDREN_END \
        ++ it; \
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
    LOG_DEBUG("Cleaning Object and its children: {}", (void*)this);

    FOR_CHILDREN_BEGIN
        child->clean();
        delete child;
    FOR_CHILDREN_END
    children_.clear();

    return 0;
}
