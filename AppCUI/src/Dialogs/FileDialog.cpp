#include "AppCUI.hpp"

using namespace AppCUI;
using namespace AppCUI::OS;
using namespace AppCUI::Utils;
using namespace AppCUI::Console;
using namespace AppCUI::Controls;
using namespace AppCUI::Dialogs;

static AppCUI::Utils::String __tmpFDString;
struct FileDialogClass
{
    Controls::Window wnd;
    Controls::Label lbPath, lbDrive, lbName, lbExt;
    Controls::ComboBox comboDrive;
    Controls::ListView files;
    Controls::TextField txName, txExt;
    Controls::Button btnOK, btnCancel;
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
    const char* s1 = control->GetItemText(item1, 0);
    const char* s2 = control->GetItemText(item2, 0);
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
    if (lbPath.GetText(__tmpFDString) == false)
        return;
    if (value == 0)
    {
        // if (__tmpFDString.PathRemoveLast())
        {
            // lbPath.SetText(__tmpFDString.GetText());
            UpdateFileList();
        }
        return;
    }
    if (value == 1)
    {
        if (OS::FileSystem::Path::Join(__tmpFDString, files.GetItemText(index, 0)))
        {
            lbPath.SetText(__tmpFDString.GetText());
            UpdateFileList();
        }
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
    LocalString<256> name;
    if ((txName.GetText(name) == false) || (name.Len() == 0))
        return;
    if (lbPath.GetText(__tmpFDString) == false)
        return;
    if (OS::FileSystem::Path::Join(__tmpFDString, name.GetText()) == false)
        return;
    bool exists = FileSystem::FileExists(__tmpFDString);
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
    FileSystem::SpecialFolder id = (FileSystem::SpecialFolder)(comboDrive.GetCurrentItemUserData().UInt32Value);
    // update lbPath with the name of the selected special folder

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
        ItemHandle itemHandle;
        try
        {
            for (const auto& fileEntry : std::filesystem::directory_iterator(p))
            {
                if (fileEntry.is_directory())
                    Utils::String::Set(size, "Folder", 32, 6);
                else
                    ConvertSizeToString((unsigned long long) fileEntry.file_size(), size);

                itemHandle = this->files.AddItem((const char*) fileEntry.path().filename().u8string().c_str(), size);
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
    if (eventType == Event::EVENT_BUTTON_CLICKED)
    {
        if (controlID == (int) Dialogs::Result::Ok)
        {
            Validate();
            return true;
        }
        wnd.Exit(controlID);
        return true;
    }
    if (eventType == Event::EVENT_WINDOW_CLOSE)
    {
        wnd.Exit((int) Dialogs::Result::Cancel);
        return true;
    }
    if (eventType == Event::EVENT_WINDOW_ACCEPT)
    {
        Validate();
        return true;
    }
    if (eventType == Event::EVENT_COMBOBOX_SELECTED_ITEM_CHANGED)
    {
        UpdateCurrentFolder();
        UpdateFileList();
        return true;
    }
    if (eventType == Event::EVENT_TEXTFIELD_VALIDATE)
    {
        UpdateFileList();
        files.SetFocus();
        return true;
    }
    if (eventType == Event::EVENT_LISTVIEW_CURRENTITEM_CHANGED)
    {
        OnCurrentItemChanged();
        return true;
    }
    if (eventType == Event::EVENT_LISTVIEW_ITEM_CLICKED)
    {
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
    lbPath.Create(&wnd, "", "x:24,y:1,w:52");
    lbDrive.Create(&wnd, "&Drive", "x:2,y:1,w:8");
    comboDrive.Create(&wnd, "x:8,y:1,w:14");
    comboDrive.SetHotKey('D');
    // populate combo box with special folders and available drivers
    LocalString<32> tempStr;
    for (auto specialFolderID : AppCUI::OS::FileSystem::AllSpecialFolders)
    {
        if (OS::FileSystem::GetSpecialFolderName(specialFolderID, tempStr))
        {            
            comboDrive.AddItem(tempStr.GetText(), ItemData{ (unsigned int) specialFolderID });
        }
    }

    comboDrive.SetCurentItemIndex(0);
    files.Create(&wnd, "x:2,y:3,w:72,h:13", ListViewFlags::SORTABLE);
    files.AddColumn("&Name", TextAlignament::Left, 41);
    files.AddColumn("&Size", TextAlignament::Right, 16);
    files.AddColumn("&Modified", TextAlignament::Right, 10);
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
    if (res == (int) Dialogs::Result::Ok)
        return __tmpFDString.GetText();
    return nullptr;
}
const char* FileDialog::ShowOpenFileWindow(const char* fileName, const char* mask, const char* path)
{
    FileDialogClass dlg;
    int res = dlg.Show(true, fileName, mask, path);
    if (res == (int) Dialogs::Result::Ok)
        return __tmpFDString.GetText();
    return nullptr;
}
