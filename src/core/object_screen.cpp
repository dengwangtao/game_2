#include "object_screen.h"
#include "comm/log_def.h"

s32 ObjectScreen::render()
{
    Object::render();

#ifdef DWT_DEBUG
    auto aabb = game_.GetCameraAABB();

    LOG_TRACE("camera aabb=[{},{},{},{}], player pos=[{},{}]",
             aabb.first.x, aabb.first.y, aabb.second.x, aabb.second.y,
             pos().x, pos().y);

    game_.draw_rect(get_render_pos(), {5, 5}, Color(255, 0, 0, 255));
#endif

    return 0;
}
