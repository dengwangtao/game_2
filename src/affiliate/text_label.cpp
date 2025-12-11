#include "text_label.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "resource_mgr.h"


s32 TextLabel::render()
{
    if (!ttf_text_)
    {
        LOG_ERROR("TextLabel render failed: ttf_text_ is null");
        return -1;
    }

    if (!parent_)
    {
        LOG_ERROR("TextLabel render failed: parent is null");
        return -2;
    }

    TTF_DrawRendererText(
        ttf_text_,
        parent_->get_render_pos().x + offset_.x,
        parent_->get_render_pos().y + offset_.y
    );

    return 0;
}

s32 TextLabel::clean()
{
    ObjectAffiliate::clean();

    if (ttf_text_) TTF_DestroyText(ttf_text_);
    return 0;
}

void TextLabel::set_text(const String& text)
{
    TTF_SetTextString(ttf_text_, text.c_str(), text.length());
    update_size();
}

void TextLabel::set_font_size(s32 font_size)
{
    font_size_ = font_size;

    auto* font = G_RESOURCE_MGR.loadResource<TTF_Font>(
        font_path_,
        font_size_
    );

    if (!font)
    {
        LOG_ERROR("Failed to load font: {}", font_path_);
        return;
    }

    TTF_SetTextFont(ttf_text_, font);

    update_size();
}

void TextLabel::set_font_path(const String& font_path)
{
    font_path_ = font_path;

    auto* font = G_RESOURCE_MGR.loadResource<TTF_Font>(
        font_path_,
        font_size_
    );

    if (!font)
    {
        LOG_ERROR("Failed to load font: {}", font_path_);
        return;
    }

    TTF_SetTextFont(ttf_text_, font);
    
    update_size();
}

void TextLabel::set_font(const String& font_path, s32 font_size)
{
    font_path_ = font_path;
    font_size_ = font_size;

    if (! ttf_text_)
    {
        ttf_text_ = game_.create_ttf_text("", font_path_, font_size_);
    }
    
    set_font_size(font_size);
}

String TextLabel::get_text() const
{
    if (ttf_text_)
    {
        return ttf_text_->text;
    }
    return "";
}

TextLabel* TextLabel::add_text(ObjectScreen* parent, const String& text,
                               const String& font_path, s32 font_size,
                                Anchor anchor)
{
    TextLabel* label = new TextLabel();
    label->init();

    label->set_parent(parent);
    label->set_font(font_path, font_size);
    label->set_text(text);
    label->set_offset_by_anchor(anchor);
    
    if (parent)
    {
        parent->add_child_safe(label);
    }
    return label;

}

void TextLabel::update_size()
{
    s32 w, h;
    TTF_GetTextSize(
        ttf_text_,
        &w,
        &h
    );

    set_size(Vec2{static_cast<f32>(w), static_cast<f32>(h)});
}
