#include "hud_text.h"
#include "affiliate/sprite.h"
#include "affiliate/text_label.h"
#include "resource_mgr.h"

s32 HUDText::render()
{
    ObjectScreen::render();
    return 0;
}

void HUDText::set_text(const String& text)
{
    if (text_label_)
    {
        text_label_->set_text(text);
    }
}

void HUDText::set_background(const String& bg_path)
{
    if (sprite_bg_)
    {
        Texture texture(bg_path);
        sprite_bg_->set_texture(texture);
    }
    else
    {
        Sprite::add_sprite<Sprite>(
            this,
            bg_path,
            Anchor::TOP_LEFT,
            1.0f
        );
    }
}

void HUDText::set_size(const Vec2& size)
{
    size_ = size;
    if (sprite_bg_) sprite_bg_->set_size(size);
}

HUDText* HUDText::add_hud_text(Object* parent, const String& text,
                               const Vec2& screen_pos, const Vec2& size,
                               const String& font_path, s32 font_size,
                               const String& bg_path, Anchor anchor)
{
    auto* hud_text = new HUDText();
    hud_text->init();

    hud_text->set_render_pos(screen_pos);

    auto bg = Sprite::add_sprite<Sprite>(hud_text, bg_path, anchor);
    hud_text->set_sprite_bg(bg);

    hud_text->set_size(size);

    auto lbl = TextLabel::add_text(hud_text, text, font_path, font_size, anchor);
    hud_text->set_text_label(lbl);


    if (parent) parent->add_child(hud_text);

    return hud_text;
}
