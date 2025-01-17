#ifndef _SDL_TERMINAL_INCLUDED_
#define _SDL_TERMINAL_INCLUDED_

#include "Internal.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <iterator>
#include <map>
#include <memory>
#include <string>

namespace AppCUI
{
namespace Internal
{
    class SDLTerminal : public AbstractTerminal
    {
      private:
        std::map<SDL_Scancode, AppCUI::Input::Key> KeyTranslation;
        std::map<SDL_Scancode, AppCUI::Input::Key> AsciiTranslation;
        AppCUI::Input::Key oldShiftState;

        SDL_Window* window;
        Uint32 windowID;
        SDL_Renderer* renderer;
        TTF_Font* font;
        size_t charWidth;
        size_t charHeight;

      public:
        virtual bool OnInit(const InitializationData& initData) override;
        virtual void OnUninit() override;
        virtual void OnFlushToScreen() override;
        virtual bool OnUpdateCursor() override;
        virtual void GetSystemEvent(AppCUI::Internal::SystemEvent& evnt) override;
        virtual bool IsEventAvailable() override;
        virtual void RestoreOriginalConsoleSettings() override;

      private:
        bool initScreen(const InitializationData& initData);
        bool initInput(const InitializationData& initData);

        bool initFont(const InitializationData& initData);

        void uninitScreen();
        void uninitInput();

        void handleMouse(SystemEvent& evt, const SDL_Event& eSdl);
        void handleKeyUp(SystemEvent& evt, const SDL_Event& eSdl);
        void handleKeyDown(SystemEvent& evt, const SDL_Event& eSdl);
    };
} // namespace Internal
} // namespace AppCUI

#endif // _SDL_TERMINAL_INCLUDED_