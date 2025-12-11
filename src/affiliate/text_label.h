#pragma once



#include "core/object_affiliate.h"
#include "comm/comm_def.h"

struct TTF_Text;

class TextLabel : public ObjectAffiliate
{

public:
    s32 render() override;
    s32 clean() override;

    void set_text(const String& text);
    void set_font_size(s32 font_size);
    void set_font_path(const String& font_path);
    void set_font(const String& font_path, s32 font_size);

    String get_text() const;
    s32 get_font_size() const { return font_size_; }
    const String& get_font_path() const { return font_path_; }

    static TextLabel* add_text(
        ObjectScreen* parent,
        const String& text,
        const String& font_path,
        s32 font_size = 16,
        Anchor anchor = Anchor::CENTER
    );

private:
    void update_size();

protected:

    TTF_Text* ttf_text_ = nullptr;
    String font_path_;
    s32 font_size_ = 16; // 默认字体大小

};