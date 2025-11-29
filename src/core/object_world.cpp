#include "object_world.h"
#include "comm/log_def.h"
#include "core/scene.h"

s32 ObjectWorld::update(s64 now_ms)
{
    ObjectScreen::update(now_ms);


    // 世界坐标可能一直不变，但是摄像机在变，导致屏幕坐标变化，需要实时更新
    // 更新屏幕坐标
    auto scene = game_.get_curr_scene();
    if (! scene)
    {
        LOG_ERROR("curr scene is null");
        return -1;
    }
    set_render_pos(scene->world2screen(world_pos_));

    return s32();
}

void ObjectWorld::set_world_pos(const Vec2 &pos)
{
    world_pos_ = pos;
    // 更新屏幕坐标
    auto scene = game_.get_curr_scene();
    if (! scene)
    {
        LOG_ERROR("curr scene is null");
        return;
    }
    set_render_pos(scene->world2screen(pos));
}

void ObjectWorld::set_render_pos(const Vec2 &pos)
{
    render_pos_ = pos;
    // 更新世界坐标
    auto scene = game_.get_curr_scene();
    if (! scene)
    {
        LOG_ERROR("curr scene is null");
        return;
    }
    set_world_pos(scene->screen2world(pos));
}
