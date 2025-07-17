#pragma once
#include "InputActions.h"
#include "IActionListener.h"
#include <unordered_map>

// Manages input bindings and dispatches input events to registered listeners.
class InputManager
{
public:
    static InputManager& Instance();
    ~InputManager();

    // Creates a new boolean key binding for a listener.
    void CreateNewBooleanKeyBinding(IActionListener* listener, std::string name, SDL_Keycode key);

    // Creates a new boolean mouse button binding for a listener.
    void CreateNewBooleanBtnBinding(IActionListener* listener, std::string name, Uint8 mouseButton);

    // Creates a new 2D axis binding for a listener.
    void CreateNewAxis2DBinding(IActionListener* listener, std::string name, SDL_Keycode positiveX = SDLK_UNKNOWN, SDL_Keycode negativeX = SDLK_UNKNOWN, SDL_Keycode positiveY = SDLK_UNKNOWN, SDL_Keycode negativeY = SDLK_UNKNOWN);

    void Update();
    void Unload();
private:
    // Binds an action to a set of keyboard keys.
    void BindActionToKeys(InputActions* action, const std::vector<SDL_Keycode>& keys);

    // Binds an action to mouse buttons.
    void BindActionToMouse(InputActions* action);

    std::unordered_map<SDL_Keycode, std::vector<InputActions*>> mActionKeyBindings;
    std::vector<InputActions*> mActionMouseBindings;
};
