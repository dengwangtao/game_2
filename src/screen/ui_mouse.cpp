#include "ui_mouse.h"
#include "affiliate/sprite.h"


s32 UIMouse::update(s64 now_ms, s64 delta_ms)
{
    timer_ += delta_ms;

    if (timer_ < 300)
    {
        sprite1_->set_active(true);
        sprite2_->set_active(false);
    }
    else if (timer_ < 600)
    {
        sprite1_->set_active(false);
        sprite2_->set_active(true);
    }
    else
    {
        timer_ = 0;
    }


    // 更新位置
    auto mouse_pos = game_.get_mouse_pos();
    set_render_pos(mouse_pos);

    return 0;
}

s32 UIMouse::render()
{
    ObjectScreen::render();

    return 0;
}

UIMouse* UIMouse::add_ui_mouse(Object* parent, const String& file_path1,
                               const String& file_path2, Anchor anchor)
{
    auto* ui_mouse = new UIMouse();
    ui_mouse->init();

    auto* sprite1 = Sprite::add_sprite<Sprite>(
        ui_mouse,
        file_path1,
        anchor,
        1.0f,
        true
    );

    auto* sprite2 = Sprite::add_sprite<Sprite>(
        ui_mouse,
        file_path2,
        anchor,
        1.0f,
        false
    );

    ui_mouse->set_sprite1(sprite1);
    ui_mouse->set_sprite2(sprite2);

    if (parent) parent->add_child_safe(ui_mouse);
    return ui_mouse;
}
