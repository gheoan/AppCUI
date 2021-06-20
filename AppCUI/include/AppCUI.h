#ifndef __APPCUI_MAIN_HEADER__
#define __APPCUI_MAIN_HEADER__

#ifdef BUILD_AS_STATIC_LIB
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT
#endif

#define CHECK(c,returnValue,format,...) { if (!(c)) return (returnValue); }
#define RETURNERROR(returnValue,format,...) { return (returnValue);}
#define CHECKBK(c,format,...) if (!(c)) break;

#define NOT_IMPLEMENTED(returnValue) { return (returnValue); }
#define LOG_INFO(format,...)
#define LOG_ERROR(format,...)

#define PATH_SEPARATOR '\\'

#define MAXVALUE(x,y)	((x)>(y)?(x):(y))
#define MINVALUE(x,y)	((x)<(y)?(x):(y))

#define COLOR(fore,back) (((unsigned int)(fore))|(((unsigned int)(back))<<4))

namespace AppCUI
{
    namespace Application
    {
        struct Config;
    };
    namespace Input
    {
        namespace Key
        {
            enum Type : unsigned int
            {
                None = 0,
                F1 = 1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
                Enter, Escape, Insert, Delete, Backspace, Tab,
                Left, Up, Down, Right, PageUp, PageDown, Home, End, Space,
                A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
                Count, // must be the last
                Alt = 0x1000,
                Ctrl = 0x2000,
                Shift = 0x4000
            };
        }
    };
    namespace Utils
    {
        class EXPORT String 
        {
            char	            *Text;
            unsigned int		Size;
            unsigned int	    Allocated;

            bool				Grow(unsigned int newSize);
        public:
            String(void);
            String(const String &s);
            ~String(void);

            // Static functions
            static unsigned int	Len         (const char *string);
            static bool		    Add         (char *destination, const char *source, unsigned int maxDestinationSize, unsigned int destinationSize = 0xFFFFFFFF, unsigned int sourceSize = 0xFFFFFFFF, unsigned int * resultedDestinationSize = nullptr);
            static bool		    Set         (char *destination, const char *source, unsigned int maxDestinationSize, unsigned int sourceSize = 0xFFFFFFFF, unsigned int * resultedDestinationSize = nullptr);
            static bool         Equals      (const char *sir1, const char *sir2, bool ignoreCase = false);
            static bool		    StartsWith  (const char *sir, const char *text, bool ignoreCase = false);
            static bool		    EndsWith    (const char *sir, const char *text, bool ignoreCase = false, unsigned int sirTextSize = 0xFFFFFFFF, unsigned int textSize = 0xFFFFFFFF);

            // Create string object
            bool			Create(unsigned int initialAllocatedBuffer = 64);
            bool			Create(const char* text);
            bool			Create(char* buffer, unsigned int bufferSize, bool emptyString = false);

            const char*		GetText() const { return Text; }
            unsigned int	Len() const { return Size; }
            unsigned int	GetAllocatedSize() const { return Allocated & 0x7FFFFFFF; }

            int				GetChar(int index) const;
            bool			SetChar(int index, char value);

            bool			Add(const char *text, unsigned int size = 0xFFFFFFFF);
            bool			Add(const String& text);
            bool			Add(const String* text);
            bool			AddChar(char ch);
            bool			AddChars(char ch, unsigned int count);

            bool			Set(const char *text, unsigned int size = 0xFFFFFFFF);
            bool			Set(const String& text);
            bool			Set(const String* text);
            bool			SetChars(char ch, unsigned int count);

            bool			SetFormat(const char *format, ...);
            bool			AddFormat(const char *format, ...);
            const char*		Format(const char *format, ...);

            bool			Realloc(unsigned int newSize);
            void			Destroy();
            bool			Truncate(unsigned int newSize);
            void			Clear();

            bool			StartsWith(const char *text, bool ignoreCase = false) const;
            bool			StartsWith(const String *text, bool ignoreCase = false) const;
            bool			StartsWith(const String &text, bool ignoreCase = false) const;
            bool			EndsWith(const char *text, bool ignoreCase = false) const;
            bool			EndsWith(const String *text, bool ignoreCase = false) const;
            bool			EndsWith(const String &text, bool ignoreCase = false) const;
            bool			Equals(const char *text, bool ignoreCase = false) const;
            bool			Equals(const String &ss, bool ignoreCase = false) const;

            
            inline String&	operator=  (const String &s) { this->Set(s); return *this; }
            inline String&  operator=  (const char * text) { this->Set(text); return *this; }
            inline operator char *() const { return Text; }
            inline operator const char *() const  { return Text; }
            inline String&  operator+= (const String &s) { this->Add(s); return *this; }
            inline String&  operator+= (const char* text) { this->Add(text); return *this; }

            inline bool     operator== (const String &s) const { return this->Equals(s); }
            inline bool		operator!= (const String &s) const { return !this->Equals(s); }
            char&		    operator[] (int poz);
            
        };
        template <int size>
        class LocalString : public String
        {
            char tempBuffer[size];
        public:
            LocalString() { Create(tempBuffer, size, true); }
        };



        class EXPORT KeyUtils
        {
        public:
            enum { KEY_SHIFT_MASK = 0x7000, KEY_SHIFT_BITS = 12, KEY_CODE_MASK = 0xFF };

            // Returns the name of the Key without modifiers
            static const char*	            GetKeyName(AppCUI::Input::Key::Type keyCode);
            static const char*	            GetKeyModifierName(AppCUI::Input::Key::Type keyCode);
            static bool			            ToString(AppCUI::Input::Key::Type keyCode, char* text, int maxTextSize);
            static bool			            ToString(AppCUI::Input::Key::Type keyCode, AppCUI::Utils::String *text);
            static bool			            ToString(AppCUI::Input::Key::Type keyCode, AppCUI::Utils::String &text);
            static AppCUI::Input::Key::Type	FromString(const char * stringRepresentation);
            static AppCUI::Input::Key::Type	FromString(AppCUI::Utils::String *text);
            static AppCUI::Input::Key::Type	FromString(AppCUI::Utils::String &text);
        };
    };


    namespace Console
    {
        namespace Color
        {
            enum Type : unsigned int
            {
                Black       = 0x00,
                DarkBlue    = 0x01,
                DarkGreen   = 0x02,
                Teal        = 0x03,
                DarkRed     = 0x04,
                Magenta     = 0x05,
                Olive       = 0x06,
                Silver      = 0x07,
                Gray        = 0x08,
                Blue        = 0x09,
                Green       = 0x0A,
                Aqua        = 0x0B,
                Red         = 0x0C,
                Pink        = 0x0D,
                Yellow      = 0x0E,
                White       = 0x0F,
                Transparent = 0x100,
                NoColor     = 0x1100
            };
        };
        namespace Alignament
        {
            enum Type : unsigned int
            {
                TopLeft = 0,
                Top,
                TopRight,
                Right,
                BottomRight,
                Bottom,
                BottomLeft,
                Left,
                Center
            };
        };
        namespace SpecialChars
        {
            enum Type : unsigned int
            {
                BoxTopLeftCornerDoubleLine = 0,
                BoxTopRightCornerDoubleLine,
                BoxBottomRightCornerDoubleLine,
                BoxBottomLeftCornerDoubleLine,
                BoxHorizontalDoubleLine,
                BoxVerticalDoubleLine,

                BoxTopLeftCornerSingleLine,
                BoxTopRightCornerSingleLine,
                BoxBottomRightCornerSingleLine,
                BoxBottomLeftCornerSingleLine,
                BoxHorizontalSingleLine,
                BoxVerticalSingleLine,

                // arrows
                ArrowUp,
                ArrowDown,
                ArrowLeft,
                ArrowRight,
                ArrowUpDown,
                ArrowLeftRight,

                // always last
                Count
            };
        }

        struct Size
        {
            unsigned int Width, Height;
            inline Size() : Width(0), Height(0) { }
            inline Size(unsigned int width, unsigned int height) : Width(width), Height(height) { }
            inline void Set(unsigned int width, unsigned int height) { Width = width; Height = height; }
        };
        struct Point
        {
            int X, Y;
            inline Point(): X(0), Y(0) {} 
            inline Point(int x, int y) : X(x), Y(y) { }
            inline void Set(int x, int y) { X = x; Y = y; }
        };
        class EXPORT Rect
        {
            int X, Y, Width, Height;
        public:
            Rect() : X(0), Y(0), Width(0), Height(0) { }
            bool                 Create(int x, int y, int width, int height, Alignament::Type align);
            void                 Create(int left, int top, int right, int bottom);
            inline bool          Contains(int x, int y) const { return (x >= this->X) && (y >= this->Y) && (x < (this->X + this->Width)) && (y < (this->Y + this->Height)); }
            inline int           GetCenterX() const { return X + (Width >> 1); }
            inline int           GetCenterY() const { return Y + (Height >> 1); }
            inline int           GetX() const { return X; }
            inline int           GetY() const { return Y; }
            inline unsigned int  GetWidth() const { return (unsigned int)Width; }
            inline unsigned int  GetHeight() const { return (unsigned int)Height; }
            inline int           GetLeft() const { return X; }
            inline int           GetTop() const { return Y; }
            inline int           GetRight() const { return X + Width - 1; }
            inline int           GetBotom() const { return X + Height - 1; }
        };

        class EXPORT Clip
        {
        public:
            struct
            {
                int				X, Y, Width, Height;
            } ClipRect;
            struct
            {
                int             X, Y;
            } ScreenPosition;            
            bool			Visible;

        public:
            Clip();
            Clip(const Clip &parent, int relative_x, int relative_y, int width, int height);
            Clip(int x, int y, int width, int height);
            void			Reset();
            void			Set(const Clip &parent, int relativeLeft, int relativeTop, int relativeRight, int relativeBottom);
            void			Set(int x, int y, int width, int height);
        };

        class EXPORT Renderer
        {
            void*   consoleRenderer;
        public:
            Renderer();
            void    Init(void* consoleRenderer);
            void    FillRect(int left, int top, int right, int bottom, int charCode, unsigned int color);
            void    FillRectWidthHeight(int x, int y, int width, int height, int charCode, unsigned int color);
            void    FillHorizontalLine(int left, int y, int right, int charCode, unsigned int color);
            void    FillHorizontalLineSize(int x, int y, int size, int charCode, unsigned int color);
            void    FillVerticalLine(int x, int top, int bottom, int charCode, unsigned int color);
            void    FillVerticalLineSize(int x, int y, int size, int charCode, unsigned int color);
            void    DrawRect(int left, int top, int right, int bottom, unsigned int color, bool doubleLine);
            void    DrawRectWidthHeight(int x, int y, int width, int height, unsigned int color, bool doubleLine);
            void    Clear(int charCode, unsigned int color);
            void    ClearWithSpecialChar(SpecialChars::Type charID, unsigned int color);
            void    WriteSingleLineText(int x, int y, const char * text, unsigned int color, int textSize = -1);
            void    WriteCharacter(int x, int y, int charCode, unsigned int color);
            void    WriteSpecialCharacter(int x, int y, SpecialChars::Type charID, unsigned int color);
        };


    };
    namespace Controls
    {
        namespace Anchor
        {
            enum Type : unsigned char
            {
                Top         = 0x01,
                Left        = 0x02,
                Bottom      = 0x04,
                Right       = 0x08,
                Center
            };
        }
        namespace Events
        {
            enum Event
            {
                EVENT_WINDOW_CLOSE,
                EVENT_WINDOW_ACCEPT,
                EVENT_TERMINATE_APPLICATION,
                EVENT_COMMAND
            };
        }
        class EXPORT Control;
        namespace Handlers
        {
            typedef void(*AfterResizeHandler) (AppCUI::Controls::Control *control, int newWidth, int newHeight, void *Context);
            typedef bool(*BeforeResizeHandler) (AppCUI::Controls::Control *control, int newWidth, int newHeight, void *Context);
            typedef void(*AfterMoveHandler) (AppCUI::Controls::Control *control, int newX, int newY, void *Context);
            typedef bool(*UpdateCommandBarHandler)(AppCUI::Controls::Control *control, void* Context);
            typedef bool(*KeyEventHandler)(AppCUI::Controls::Control *control, int KeyCode, int AsciiCode, void *Context);
            typedef void(*PaintHandler)(AppCUI::Controls::Control *control, void *Context);
            typedef void(*OnFocusHandler)(AppCUI::Controls::Control *control, void *Context);
            typedef bool(*EventHandler)(AppCUI::Controls::Control *control, const void* sender, AppCUI::Controls::Events::Event eventType, int controlID, void *Context);
            typedef void(*MousePressedHandler) (AppCUI::Controls::Control *control, int x, int y, int buttonState, void *Context);
            typedef void(*MouseReleasedHandler) (AppCUI::Controls::Control *control, int x, int y, int buttonState, void *Context);
        }

        class EXPORT Control
        {
        public:
            void*			Context;
        protected:
            bool			IsMouseInControl(int x, int y);
            bool			SetMargins(int left, int top, int right, int bottom);
            bool			Init(Control *parent, const char *text, const char * layout, bool computeHotKey = false);
        public:
            Control();
            bool			AddControl(Control* control);
            bool			RemoveControl(Control * control);
            bool			RemoveControl(unsigned int index);

            bool			IsInitialized();

            // coordonates
            int				GetX();
            int				GetY();
            int				GetWidth();
            int				GetHeight();
            void            GetSize(AppCUI::Console::Size & size);
            void            GetClientSize(AppCUI::Console::Size & size);
            void			MoveTo(int newX, int newY);
            bool			Resize(int newWidth, int newHeight);
            void			RecomputeLayout();

            // groups
            int				GetGroup();
            void			SetGroup(int newGroupID);
            void			ClearGroup();

            // hot key
            bool			SetHotKey(char hotKey);
            bool            SetHotKeyFromText(const char * text, bool clearCurrentHotKey = true);
            Input::Key::Type GetHotKey();
            void			ClearHotKey();

            // status
            void			SetEnabled(bool value);
            void			SetVisible(bool value);
            void			SetChecked(bool value);
            bool			IsEnabled();
            bool			IsVisible();
            bool			IsChecked();
            bool			HasFocus();
            bool            IsMouseOver();

            // childern and parent
            Control*		GetParent();
            Control**		GetChildrenList();
            Control*		GetChild(unsigned int index);
            unsigned int	GetChildernCount();
            bool			GetChildIndex(Control *control, unsigned int &index);

            // Events
            void			RaiseEvent(Events::Event eventType);

            // focus
            bool			SetFocus();
            void			SetControlID(int newID);

            // Text
            bool			SetText(const char * text);
            bool			SetText(AppCUI::Utils::String *text);
            bool			SetText(AppCUI::Utils::String &text);
            const char*		GetText();
            bool			GetText(AppCUI::Utils::String *text);
            bool			GetText(AppCUI::Utils::String &text);

            // handlere
            void			SetOnBeforeResizeHandler(Handlers::BeforeResizeHandler handler, void *Context = nullptr);
            void			SetOnAfterResizeHandler(Handlers::AfterResizeHandler handler, void *Context = nullptr);
            void			SetOnAfterMoveHandler(Handlers::AfterMoveHandler handler, void *Context = nullptr);
            void			SetOnUpdateCommandBarHandler(Handlers::UpdateCommandBarHandler handler, void* Context = nullptr);
            void			SetOnKeyEventHandler(Handlers::KeyEventHandler handler, void* Context = nullptr);
            void			SetPaintHandler(Handlers::PaintHandler handler, void* Context = nullptr);
            void			SetOnFocusHandler(Handlers::OnFocusHandler handler, void* Context = nullptr);
            void			SetOnLoseFocusHandler(Handlers::OnFocusHandler handler, void* Context = nullptr);
            void			SetEventHandler(Handlers::EventHandler handler, void* Context = nullptr);
            void			SetMousePressedHandler(Handlers::MousePressedHandler handler, void* Context = nullptr);
            void			SetMouseReleasedHandler(Handlers::MouseReleasedHandler handler, void* Context = nullptr);
            void			SetMouseHandler(Handlers::MousePressedHandler mousePressedHandler, Handlers::MouseReleasedHandler mouseReleasedHandler, void *Context = nullptr);

            // paint
            virtual void	Paint(Console::Renderer & renderer);

            // Evenimente
            virtual bool	OnKeyEvent(AppCUI::Input::Key::Type keyCode, char AsciiCode);
            virtual void	OnHotKey();
            virtual void	OnFocus();
            virtual void	OnLoseFocus();

            virtual void	OnMousePressed(int x, int y, int Button);
            virtual void	OnMouseReleased(int x, int y, int Button);
            virtual bool	OnMouseDrag(int x, int y, int Button);
            
            virtual bool    OnMouseEnter();
            virtual bool    OnMouseOver(int x, int y);
            virtual bool    OnMouseLeave();

            virtual void	OnMouseWheel(int direction);
            virtual bool	OnEvent(const void* sender, Events::Event eventType, int controlID);
            virtual bool	OnUpdateCommandBar();

            virtual bool	OnBeforeResize(int newWidth, int newHeight);
            virtual void	OnAfterResize(int newWidth, int newHeight);
            virtual bool	OnBeforeAddControl(Control *ctrl);
            virtual void	OnAfterAddControl(Control *ctrl);
            virtual bool	OnBeforeSetText(const char * text);
            virtual void	OnAfterSetText(const char * text);

            virtual ~Control();

        };

        namespace WindowFlags
        {
            enum Type : unsigned int
            {
                NONE = 0,
                SIZEABLE = 0x000100,
                NOTIFYBOX = 0x000200,
                ERRORBOX = 0x000400,
                WARNINGBOX = 0x000800,
                NOCLOSEBUTTON = 0x001000,
                FIXED = 0x004000,
                CENTERED = 0x008000,
                MAXIMIZED = 0x010000
            };
        }
        class EXPORT Window : public Control {
        public:
            bool	Create(const char* text, const char * layout, WindowFlags::Type windowsFlags);
            void	Paint(Console::Renderer & renderer) override;
            void	OnMousePressed(int x, int y, int Button) override;
            void	OnMouseReleased(int x, int y, int Button) override;
            bool	OnMouseDrag(int x, int y, int Button) override;
            bool    OnMouseOver(int x, int y) override;
            bool    OnMouseLeave() override;
            int		Show();
            int		GetDialogResult();
            bool	MaximizeRestore();
            bool	OnBeforeResize(int newWidth, int newHeight) override;
            void	OnAfterResize(int newWidth, int newHeight) override;
            bool	CenterScreen();
            bool	OnKeyEvent(AppCUI::Input::Key::Type keyCode, char AsciiCode) override;
            bool	Exit(int dialogResult);
            bool	IsWindowInResizeMode();

            virtual ~Window();
        };

    };

    namespace Application
    {
        namespace Flags
        {
            enum Type : unsigned int
            {
                NONE            = 0,
                HAS_COMMANDBAR  = 0x00000001,
            };
        }

        struct Config
        {
            struct {
                int             DesktopFillCharacterCode;
                unsigned int    Color;
            } Desktop;
            struct {
                unsigned int BackgroundColor;
                unsigned int ShiftKeysColor;
                struct {
                    unsigned int KeyColor;
                    unsigned int NameColor;
                } Normal, Hover, Pressed;
            } CommandBar;
            struct {
                unsigned int    ActiveColor;
                unsigned int    InactiveColor;
                unsigned int    TitleActiveColor;
                unsigned int    TitleInactiveColor;
                unsigned int    ControlButtonColor;
                unsigned int    ControlButtonInactiveColor;
                unsigned int    ControlButtonHoverColor;
                unsigned int    ControlButtonPressedColor;
            } Window, DialogError, DialogNotify, DialogWarning;

            void SetDarkTheme();
        };
        typedef             void(*EventHandler)(const void* sender, AppCUI::Controls::Events::Event eventType, int controlID);

        EXPORT Config*      GetAppConfig();
        EXPORT bool         Init(Application::Flags::Type flags = Application::Flags::NONE, EventHandler eventCallback = nullptr);
        EXPORT bool         Run();
        EXPORT bool         AddWindow(AppCUI::Controls::Window * wnd);
        EXPORT bool         GetApplicationSize(AppCUI::Console::Size & size);
        EXPORT bool         GetDesktopSize(AppCUI::Console::Size & size);
        EXPORT void         Repaint();
        EXPORT void         RecomputeControlsLayout();
        EXPORT void         RaiseEvent(AppCUI::Controls::Control *control, AppCUI::Controls::Control *sourceControl, AppCUI::Controls::Events::Event eventType, int controlID);
        EXPORT bool         SetCommand(AppCUI::Input::Key::Type keyCode, const char* Name, int CommandID);
        EXPORT void         Close();
    };
}

// inline OR operator for flags
inline constexpr AppCUI::Application::Flags::Type operator|(AppCUI::Application::Flags::Type f1, AppCUI::Application::Flags::Type f2)
{
    return static_cast<AppCUI::Application::Flags::Type>(static_cast<unsigned int>(f1) | static_cast<unsigned int>(f2));
} 
inline constexpr AppCUI::Input::Key::Type operator|(AppCUI::Input::Key::Type f1, AppCUI::Input::Key::Type f2)
{
    return static_cast<AppCUI::Input::Key::Type>(static_cast<unsigned int>(f1) | static_cast<unsigned int>(f2));
}
inline constexpr void operator|=(AppCUI::Input::Key::Type & f1, AppCUI::Input::Key::Type f2)
{
    f1 = static_cast<AppCUI::Input::Key::Type>(static_cast<unsigned int>(f1) | static_cast<unsigned int>(f2));
}
inline constexpr AppCUI::Controls::WindowFlags::Type operator|(AppCUI::Controls::WindowFlags::Type f1, AppCUI::Controls::WindowFlags::Type f2)
{
    return static_cast<AppCUI::Controls::WindowFlags::Type>(static_cast<unsigned int>(f1) | static_cast<unsigned int>(f2));
}

#endif