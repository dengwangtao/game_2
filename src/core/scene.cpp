#include "scene.h"

#include "object_world.h"
#include "object_screen.h"

#include "comm/log_def.h"

s32 Scene::init()
{
    Object::init();
    return 0;
}

s32 Scene::handle_events(SDL_Event& event)
{
    Object::handle_events(event);

    for (auto* obj_screen : childern_screen_)
    {
        obj_screen->handle_events(event);
    }

    for (auto* obj_world : childern_world_)
    {
        obj_world->handle_events(event);
    }
    return 0;
}
s32 Scene::update(s64 now_ms, s64 delta_ms)
{
    Object::update(now_ms, delta_ms);

    for (auto* obj_world : childern_world_)
    {
        obj_world->update(now_ms, delta_ms);
    }

    for (auto* obj_screen : childern_screen_)
    {
        obj_screen->update(now_ms, delta_ms);
    }
    return 0;
}



s32 Scene::render()
{
    Object::render();

    for (auto* obj_world : childern_world_)
    {
        obj_world->render();
    }

    for (auto* obj_screen : childern_screen_)
    {
        obj_screen->render();
    }

    return 0;
}

s32 Scene::clean()
{
    Object::clean();

    for (auto* obj_screen : childern_screen_)
    {
        obj_screen->clean();
        delete obj_screen;
    }
    childern_screen_.clear();

    for (auto* obj_world : childern_world_)
    {
        obj_world->clean();
        delete obj_world;
    }
    childern_world_.clear();
    return 0;
}


void Scene::add_child(Object* obj)
{
    if (auto* obj_world = dynamic_cast<ObjectWorld*>(obj))
    {
        childern_world_.insert(obj_world);
        LOG_TRACE("Added ObjectWorld child");
    }
    else if (auto* obj_screen = dynamic_cast<ObjectScreen*>(obj))
    {
        childern_screen_.insert(obj_screen);
        LOG_TRACE("Added ObjectScreen child");
    }
    else
    {
        children_.insert(obj);
        LOG_TRACE("Added generic child");
    }
}

void Scene::remove_child(Object* obj)
{
    if (auto* obj_world = dynamic_cast<ObjectWorld*>(obj))
    {
        childern_world_.erase(obj_world);
        LOG_TRACE("Removed ObjectWorld child");
    }
    else if (auto* obj_screen = dynamic_cast<ObjectScreen*>(obj))
    {
        childern_screen_.erase(obj_screen);
        LOG_TRACE("Removed ObjectScreen child");
    }
    else
    {
        children_.erase(obj);
        LOG_TRACE("Removed generic child");
    }
}


