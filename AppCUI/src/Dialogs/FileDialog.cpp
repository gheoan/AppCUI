#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::OS;
using namespace AppCUI::Utils;
using namespace AppCUI::Console;
using namespace AppCUI::Controls;
using namespace AppCUI::Dialogs;


struct FileDialogClass
{
    Controls::Window wnd;
    Controls::Label lbPath, lbDrive, lbName, lbExt;
    Controls::ComboBox comboDrive;
    Controls::ListView files;
    Controls::TextField txName, txExt;
    Controls::Button btnOK, btnCancel;
    std::vector<std::pair<std::string, std::filesystem::path>> specialFolders;
    const char* defaultFileName;
    bool openDialog;

    int Show(bool open, const char* fileName, const char* ext, const char* _path);
    void UpdateCurrentFolder();
    void UpdateFileList();
    bool OnEventHandler(const void* sender, AppCUI::Controls::Event eventType, int controlID);
    void Validate();
    void OnClickedOnItem();
    void OnCurrentItemChanged();
};

void ConvertSizeToString(unsigned long long size, char result[32])
{
    result[31] = 0;
    int poz    = 30;
    int cnt    = 0;
    do
    {
        result[poz--] = (size % 10) + '0';
        cnt++;
        size = size / 10;
        if ((cnt == 3) && (poz > 0) && (size>0))
        {
            result[poz--] = ',';
            cnt           = 0;
        }
        
    } while ((size > 0) && (poz > 0));
    while (poz >= 0)
    {
        result[poz--] = ' ';
    }
}


int FileDialog_ListViewItemComparer(ListView* control, ItemHandle item1, ItemHandle item2, unsigned int columnIndex, void* Context)
{
    unsigned long long v1 = control->GetItemData(item1)->UInt64Value;
    unsigned long long v2 = control->GetItemData(item2)->UInt64Value;
    if (v1 < v2)
        return -1;
    if (v1 > v2)
        return 1;
    const char* s1 = control->GetItemText(item1, columnIndex);
    const char* s2 = control->GetItemText(item2, columnIndex);
    return Utils::String::Compare(s1, s2, true);
}
bool FileDialog_EventHandler(
      Control* control, const void* sender, AppCUI::Controls::Event eventType, int controlID, void* Context)
{
    return ((FileDialogClass*) control)->OnEventHandler(sender, eventType, controlID);
}
void FileDialogClass::OnClickedOnItem()
{
    int index = files.GetCurrentItem();
    if (index < 0)
        return;
    unsigned int value = (int) files.GetItemData(index)->UInt32Value;
    LocalString<256> s;
    if (lbPath.GetText(s) == false)
        return;
    std::filesystem::path p = s.GetText();
    if (value == 0)
    {
        //GDT: reanalize
        lbPath.SetText((const char *)p.parent_path().u8string().c_str());
        UpdateFileList();
        return;
    }
    if (value == 1)
    {
        p /= files.GetItemText(index, 0);
        // GDT: reanalize
        lbPath.SetText((const char*) p.u8string().c_str());
        UpdateFileList();
        return;
    }
    if (value == 2)
        Validate();
}
void FileDialogClass::OnCurrentItemChanged()
{
    int index = files.GetCurrentItem();
    if (index < 0)
        return;
    unsigned int value = files.GetItemData(index)->UInt32Value;
    if (value == 2)
        txName.SetText(files.GetItemText(index, 0));
    else
        txName.SetText(defaultFileName);
}
void FileDialogClass::Validate()
{
    LocalString<256> name, pth;
    if ((txName.GetText(name) == false) || (name.Len() == 0))
        return;
    if (lbPath.GetText(pth) == false)
        return;
    std::filesystem::path result = pth.GetText();
    result /= name.GetText();
    bool exists = std::filesystem::exists(result);
    if (openDialog)
    {
        if (exists == false)
        {
            MessageBox::ShowError("Error", "Selected file does not exists !");
            return;
        }
    }
    else
    {
        if (exists)
        {
            if (MessageBox::ShowOkCancel("Overwrite", "Current file already exists. Overwrite ?") !=
                Dialogs::Result::Ok)
                return;
        }
    }
    // all is ok
    wnd.Exit((int) Dialogs::Result::Ok);
}
void FileDialogClass::UpdateCurrentFolder()
{
    unsigned int idx = comboDrive.GetCurrentItemUserData().UInt32Value;
    // update lbPath with the name of the selected special folder
    lbPath.SetText((const char *)specialFolders[idx].second.u8string().c_str());
    UpdateFileList();

}
void FileDialogClass::UpdateFileList()
{
    files.DeleteAllItems();
    LocalString<256> s_p;     
    if (lbPath.GetText(s_p))
    {
        std::filesystem::path p = s_p.GetText();
        if (p != p.root_path())
        {
            files.AddItem("..", "UP-DIR");
            files.SetItemData(0, 0);
        }
        char size[32];
        char time_rep[64];
        ItemHandle itemHandle;
        try
        {
            for (const auto& fileEntry : std::filesystem::directory_iterator(p))
            {
                if (fileEntry.is_directory())
                    Utils::String::Set(size, "Folder", 32, 6);
                else
                    ConvertSizeToString((unsigned long long) fileEntry.file_size(), size);

                // review - partically corect --> need to find a proper time representation
                auto t_c = std::chrono::system_clock::to_time_t(
                      std::chrono::clock_cast<std::chrono::system_clock>(fileEntry.last_write_time()));
                std::strftime(time_rep, sizeof(time_rep), "%Y-%m-%d  %H:%M:%S", std::localtime(&t_c));
                
                // review !!!! ==> should not be const char * ==> should be path!
                itemHandle = this->files.AddItem((const char*) fileEntry.path().filename().u8string().c_str(), size, time_rep);
                if (fileEntry.is_directory())
                {
                    this->files.SetItemColor(itemHandle, ColorPair{ Color::White, Color::Transparent });
                    this->files.SetItemData(itemHandle, ItemData{ 1 });
                }
                else
                {
                    this->files.SetItemColor(itemHandle, ColorPair{ Color::Gray, Color::Transparent });
                    this->files.SetItemData(itemHandle, ItemData{ 2 });
                }
            }
        }
        catch (...)
        {
            // for the moment skip
        }
        files.Sort();
    }
}
bool FileDialogClass::OnEventHandler(const void* sender, AppCUI::Controls::Event eventType, int controlID)
{
    switch (eventType)
    {
        case Event::EVENT_BUTTON_CLICKED:
            if (controlID == (int) Dialogs::Result::Ok)
                Validate();
            else
                wnd.Exit(controlID);
            return true;
        case Event::EVENT_WINDOW_CLOSE:
            wnd.Exit((int) Dialogs::Result::Cancel);
            return true;
        case Event::EVENT_WINDOW_ACCEPT:
            Validate();
            return true;
        case Event::EVENT_COMBOBOX_SELECTED_ITEM_CHANGED:
            UpdateCurrentFolder();
            UpdateFileList();
            return true;
        case Event::EVENT_TEXTFIELD_VALIDATE:
            UpdateFileList();
            files.SetFocus();
            return true;
        case Event::EVENT_LISTVIEW_CURRENTITEM_CHANGED:
            OnCurrentItemChanged();
            return true;
        case Event::EVENT_LISTVIEW_ITEM_CLICKED:
            OnClickedOnItem();
            return true;
    }
    return true;
}
int FileDialogClass::Show(bool open, const char* fileName, const char* ext, const char* _path)
{
    if (fileName == nullptr)
        fileName = "";
    if ((ext == nullptr) || (Utils::String::Len(ext) == 0))
        ext = "*.*";

    defaultFileName = fileName;
    openDialog      = open;
    if (open)
        wnd.Create("Open", "w:78,h:23,a:c");
    else
        wnd.Create("Save", "w:78,h:23,a:c");
    wnd.SetEventHandler(FileDialog_EventHandler, this);
    lbPath.Create(&wnd, "", "x:2,y:2,w:63");
    lbDrive.Create(&wnd, "&Location", "x:2,y:1,w:8");
    comboDrive.Create(&wnd, "x:12,y:1,w:61");
    comboDrive.SetHotKey('L');
    // populate combo box with special folders and available drivers
    AppCUI::OS::GetSpecialFolders(this->specialFolders);
    for (unsigned int index = 0; index < this->specialFolders.size();index++)
    {
        comboDrive.AddItem(this->specialFolders[index].first.c_str(), ItemData{ index });
    }

    comboDrive.SetCurentItemIndex(0);
    files.Create(&wnd, "x:2,y:3,w:72,h:13", ListViewFlags::SORTABLE);
    files.AddColumn("&Name", TextAlignament::Left, 31);
    files.AddColumn("&Size", TextAlignament::Right, 16);
    files.AddColumn("&Modified", TextAlignament::Center, 20);
    files.SetItemCompareFunction(FileDialog_ListViewItemComparer, this);

    lbName.Create(&wnd, "File &Name", "x:2,y:17,w:10");
    txName.Create(&wnd, fileName, "x:13,y:17,w:47");
    txName.SetHotKey('N');
    lbExt.Create(&wnd, "File &Mask", "x:2,y:19,w:10");
    txExt.Create(&wnd, ext, "x:13,y:19,w:47", TextFieldFlags::PROCESS_ENTER);
    txExt.SetHotKey('M');

    btnOK.Create(&wnd, "&Ok", "x:62,y:17,w:13", (int) Dialogs::Result::Ok);
    btnCancel.Create(&wnd, "&Cancel", "x:62,y:19,w:13", (int) Dialogs::Result::Cancel);
    if ((_path == nullptr) || (Utils::String::Len(_path) == 0))
        UpdateCurrentFolder();
    else
        lbPath.SetText(_path);
    UpdateFileList();
    txName.SetFocus();
    return wnd.Show();
}

const char* FileDialog::ShowSaveFileWindow(const char* fileName, const char* mask, const char* path)
{
    FileDialogClass dlg;
    int res = dlg.Show(false, fileName, mask, path);
    //if (res == (int) Dialogs::Result::Ok)
    //    return __tmpFDString.GetText();
    return nullptr;
}
const char* FileDialog::ShowOpenFileWindow(const char* fileName, const char* mask, const char* path)
{
    FileDialogClass dlg;
    int res = dlg.Show(true, fileName, mask, path);
    //if (res == (int) Dialogs::Result::Ok)
    //    return __tmpFDString.GetText();
    return nullptr;
}
