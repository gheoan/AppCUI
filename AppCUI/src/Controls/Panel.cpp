#include "ControlContext.hpp"

using namespace AppCUI::Controls;
using namespace AppCUI::Graphics;
using namespace AppCUI::Input;

#define PANEL_ATTR_BORDER 1024

bool Panel::Create(Control* parent, const AppCUI::Utils::ConstString& caption, const std::string_view& layout)
{
    CONTROL_INIT_CONTEXT(ControlContext);
    CHECK(Init(parent, caption, layout, false), false, "Failed to create panel !");
    CREATE_CONTROL_CONTEXT(this, Members, false);
    Members->Flags = GATTR_VISIBLE | GATTR_ENABLE | PANEL_ATTR_BORDER;
    CHECK(SetMargins(1, 1, 1, 1), false, "Failed to set margins !");
    return true;
}
bool Panel::Create(Control* parent, const std::string_view& layout)
{
    CONTROL_INIT_CONTEXT(ControlContext);
    CHECK(Init(parent, "", layout, false), false, "Failed to create panel !");
    CREATE_CONTROL_CONTEXT(this, Members, false);
    Members->Flags = GATTR_VISIBLE | GATTR_ENABLE;
    return true;
}
void Panel::Paint(Graphics::Renderer& renderer)
{
    CREATE_CONTROL_CONTEXT(this, Members, );
    renderer.Clear(' ', Members->Cfg->Panel.NormalColor);
    if (Members->Flags & PANEL_ATTR_BORDER)
    {
        renderer.DrawRectSize(
              0, 0, Members->Layout.Width, Members->Layout.Height, Members->Cfg->Panel.NormalColor, false);
        if (Members->Layout.Width > 6)
        {
            WriteTextParams params(
                  WriteTextFlags::SingleLine | WriteTextFlags::ClipToWidth | WriteTextFlags::OverwriteColors |
                  WriteTextFlags::LeftMargin | WriteTextFlags::RightMargin, TextAlignament::Left);
            params.X              = 3;
            params.Y              = 0;
            params.Color          = Members->Cfg->Panel.NormalColor;
            params.Width          = Members->Layout.Width - 6;
            renderer.WriteText(Members->Text, params);
        }
    }
}