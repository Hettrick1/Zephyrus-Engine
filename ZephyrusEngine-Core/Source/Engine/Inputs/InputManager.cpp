#include "InputManager.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "SceneManager.h"

InputManager& InputManager::Instance()
{
    static InputManager instance;
    return instance;
}

InputManager::~InputManager()
{
    for (auto it = mActionKeyBindings.begin(); it != mActionKeyBindings.end(); ++it) {
        auto& key = it->first;
        auto& actions = it->second;
        for (auto& action : actions)
        {
            if (action != nullptr)
            {
                action = nullptr;
            }
        }
        actions.clear();
    }
    mActionKeyBindings.clear();
}

void InputManager::CreateNewBooleanKeyBinding(IActionListener* listener, std::string name, SDL_Keycode key)
{
    if (mActionKeyBindings.find(key) != mActionKeyBindings.end()) {
        std::vector<InputActions*> actionsToBind = mActionKeyBindings[key];
        for (InputActions* action : actionsToBind) {
            action->AddListener(listener);
        }
    }
    else {
        BooleanActions* newAction = new BooleanActions(key, name);
        newAction->AddListener(listener);
        BindActionToKeys(newAction, { key });
    }
}

void InputManager::CreateNewBooleanBtnBinding(IActionListener* listener, std::string name, Uint8 mouseButton)
{
    BooleanActions* newAction = new BooleanActions(mouseButton, name);
    newAction->AddListener(listener);
    mActionMouseBindings.push_back(newAction);
}

void InputManager::CreateNewAxis2DBinding(IActionListener* listener, std::string name, SDL_Keycode positiveX, SDL_Keycode negativeX, SDL_Keycode positiveY, SDL_Keycode negativeY)
{
    bool hasKeyboardBinding = (positiveX != SDLK_UNKNOWN && negativeX != SDLK_UNKNOWN && positiveY != SDLK_UNKNOWN && negativeY != SDLK_UNKNOWN);

    if (hasKeyboardBinding) {
        Axis2DAction* newAction = new Axis2DAction(positiveX, negativeX, positiveY, negativeY, name);
        newAction->AddListener(listener);
        BindActionToKeys(newAction, { positiveX, negativeX, positiveY, negativeY });
    }
    else {
        Axis2DAction* newAction = new Axis2DAction(name);
        newAction->AddListener(listener);
        BindActionToMouse(newAction);
    }
}

void InputManager::BindActionToKeys(InputActions* action, const std::vector<SDL_Keycode>& keys)
{
    for (auto& key : keys) {
        mActionKeyBindings[key].push_back(action);
    }
}

void InputManager::BindActionToMouse(InputActions* action)
{
    mActionMouseBindings.push_back(action);
}

void InputManager::Update()
{
    if (mActionKeyBindings.empty() && mActionMouseBindings.empty()) 
    {
        return;
    }
    for (auto it = mActionKeyBindings.begin(); it != mActionKeyBindings.end(); ++it) 
    {
        if (!SceneManager::mIsSceneLoaded)
        {
            return;
        }
        auto& key = it->first;        
        auto& actions = it->second;
        for (auto* action : actions) 
        {
            action->Update();
            if (!SceneManager::mIsSceneLoaded)
            {
                return;
            }
        }
    }
    for (InputActions* action : mActionMouseBindings)
    {
        action->Update();
        if (!SceneManager::mIsSceneLoaded)
        {
            return;
        }
    }
}

void InputManager::Unload()
{
    for (auto it = mActionKeyBindings.begin(); it != mActionKeyBindings.end(); ++it)
    {
        auto& key = it->first;
        auto& actions = it->second;
        for (auto* action : actions)
        {
            action->ClearListeners();
        }
    }
    mActionKeyBindings.clear();
    for (InputActions* action : mActionMouseBindings)
    {
        action->ClearListeners();
    }
    mActionMouseBindings.clear();
}
