#pragma once



#include "core/object_screen.h"


class Sprite;

class UIMouse : public ObjectScreen
{

public:
    s32 update(s64 now_ms, s64 delta_ms) override;
    s32 render() override;


    Sprite* get_sprite1() const { return sprite1_; }
    Sprite* get_sprite2() const { return sprite2_; }
    void set_sprite1(Sprite* sprite) { sprite1_ = sprite; }
    void set_sprite2(Sprite* sprite) { sprite2_ = sprite; }

    s64 get_timer() const { return timer_; }
    void set_timer(s64 timer) { timer_ = timer; }



    static UIMouse* add_ui_mouse(
        Object* parent,
        const String& file_path1,
        const String& file_path2,
        Anchor anchor = Anchor::CENTER
    );

protected:
    Sprite* sprite1_ = nullptr;
    Sprite* sprite2_ = nullptr;

    s64 timer_ = 0;
};