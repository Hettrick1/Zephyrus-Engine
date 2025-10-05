#pragma once
#include "InputActions.h"
#include "IActionListener.h"
#include <unordered_map>

class ISceneContext;

// Manages input bindings and dispatches input events to registered listeners.
namespace Zephyrus::Inputs {
    class InputManager
    {
    private:
        ISceneContext* mContext;

        // Binds an action to a set of keyboard keys.
        void BindActionToKeys(InputActions* pAction, const std::vector<SDL_Keycode>& pKeys);

        // Binds an action to mouse buttons.
        void BindActionToMouse(InputActions* pAction);

        std::unordered_map<SDL_Keycode, std::vector<InputActions*>> mActionKeyBindings;
        std::vector<InputActions*> mActionMouseBindings;

    public:
        static InputManager& Instance();
        ~InputManager();

        // Creates a new boolean key binding for a listener.
        void CreateNewBooleanKeyBinding(IActionListener* pListener, const std::string& pName, SDL_Keycode pKey);

        // Creates a new boolean mouse button binding for a listener.
        void CreateNewBooleanBtnBinding(IActionListener* pListener, const std::string& pName, Uint8 pMouseButton);

        // Creates a new 2D axis binding for a listener.
        void CreateNewAxis2DBinding(IActionListener* pListener, const std::string& pName, SDL_Keycode pPositiveX = SDLK_UNKNOWN, SDL_Keycode pNegativeX = SDLK_UNKNOWN, SDL_Keycode pPositiveY = SDLK_UNKNOWN, SDL_Keycode pNegativeY = SDLK_UNKNOWN);

        void Update();
        void Unload();

        void SetContext(ISceneContext* pContext);
    };
}