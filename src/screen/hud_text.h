#pragma once

#include "core/object_screen.h"
#include "comm/comm_def.h"

class TextLabel;
class Sprite;

class HUDText : public ObjectScreen
{

public:
    s32 render() override;

    void set_text(const String& text);
    void set_background(const String& bg_path);


    void set_sprite_bg(Sprite* bg) { sprite_bg_ = bg; }
    Sprite* get_sprite_bg() const { return sprite_bg_; }

    void set_text_label(TextLabel* lbl) { text_label_ = lbl; }
    TextLabel* get_text_label() const { return text_label_; }

    Vec2 get_size() const { return size_; }
    void set_size(const Vec2& size);


    static HUDText* add_hud_text(
        Object* parent,
        const String& text,
        const Vec2& screen_pos,
        const Vec2& size,
        const String& font_path,
        s32 font_size,
        const String& bg_path,
        Anchor anchor = Anchor::CENTER
    );

private:
    Sprite* sprite_bg_ = nullptr;
    TextLabel* text_label_ = nullptr;
    Vec2 size_ = Vec2(0);

};