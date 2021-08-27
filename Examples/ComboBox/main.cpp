﻿#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;

class MyWin : public AppCUI::Controls::Window
{
    ComboBox cb1, cb2, cb3;
    Label inf, col, inf2, inf3;

  public:
    MyWin()
    {
        this->Create("ComboBox example", "a:c,w:60,h:11");
        inf.Create(this, "Select a color", "x:1,y:1,w:15");
        col.Create(this, "", "x:1,y:2,w:15");
        cb1.Create(this, "x:22,y:1,w:30", "White,Blue,Red,Aqua,Metal,Yellow,Green,Orange");
        inf2.Create(this, "Select a word", "x:1,y:4,w:15");
        cb2.Create(this, "x:22,y:4,w:30", u8"Déjà vu,Schön,Groß,Fähig,Любовь,Кошка,Улыбаться");
        inf3.Create(this, "Select a vehicle", "x:1,y:7,w:18");
        cb3.Create(this, "x:22,y:7,w:30");
        cb3.AddSeparator("Cars");
        cb3.AddItem("Mercedes");
        cb3.AddItem("Skoda");
        cb3.AddItem("Toyota");
        cb3.AddItem("Ford");
        cb3.AddSeparator("Motorcycles");
        cb3.AddItem("BMW");
        cb3.AddItem("Ducatti");
        
    }
    bool OnEvent(const void* sender, Event eventType, int controlID) override
    {
        if (eventType == Event::EVENT_WINDOW_CLOSE)
        {
            Application::Close();
            return true;
        }
        if (eventType == Event::EVENT_COMBOBOX_SELECTED_ITEM_CHANGED)
        {
            // GDT: to rethink
            //col.SetText(cb1.GetUnsafeCurrentItemText());
        }
        return false;
    }
};

int main()
{
    Log::ToOutputDebugString();
    if (!Application::Init())
        return 1;
    Application::AddWindow(new MyWin());
    Application::Run();
    return 0;
}
