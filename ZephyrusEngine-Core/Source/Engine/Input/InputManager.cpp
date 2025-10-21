#include "InputManager.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "Scene.h"
#include "ISceneContext.h"

namespace Zephyrus::Inputs {
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

    void InputManager::CreateNewBooleanKeyBinding(IActionListener* pListener, const std::string& pName, SDL_Keycode pKey)
    {
        if (mActionKeyBindings.find(pKey) != mActionKeyBindings.end()) {
            std::vector<InputActions*> actionsToBind = mActionKeyBindings[pKey];
            for (InputActions* action : actionsToBind) {
                action->AddListener(pListener);
            }
        }
        else {
            BooleanActions* newAction = new BooleanActions(pKey, pName);
            newAction->AddListener(pListener);
            BindActionToKeys(newAction, { pKey });
        }
    }

    void InputManager::CreateNewBooleanBtnBinding(IActionListener* pListener, const std::string& pName, Uint8 pMouseButton)
    {
        BooleanActions* newAction = new BooleanActions(pMouseButton, pName);
        newAction->AddListener(pListener);
        mActionMouseBindings.push_back(newAction);
    }

    void InputManager::CreateNewAxis2DBinding(IActionListener* pListener, const std::string& pName, SDL_Keycode pPositiveX, SDL_Keycode pNegativeX, SDL_Keycode pPositiveY, SDL_Keycode pNegativeY)
    {
        bool hasKeyboardBinding = (pPositiveX != SDLK_UNKNOWN && pNegativeX != SDLK_UNKNOWN && pPositiveY != SDLK_UNKNOWN && pNegativeY != SDLK_UNKNOWN);

        if (hasKeyboardBinding) {
            Axis2DAction* newAction = new Axis2DAction(pPositiveX, pNegativeX, pPositiveY, pNegativeY, pName);
            newAction->AddListener(pListener);
            BindActionToKeys(newAction, { pPositiveX, pNegativeX, pPositiveY, pNegativeY });
        }
        else {
            Axis2DAction* newAction = new Axis2DAction(pName);
            newAction->AddListener(pListener);
            BindActionToMouse(newAction);
        }
    }

    void InputManager::BindActionToKeys(InputActions* pAction, const std::vector<SDL_Keycode>& pKeys)
    {
        for (auto& key : pKeys) {
            mActionKeyBindings[key].push_back(pAction);
        }
    }

    void InputManager::BindActionToMouse(InputActions* pAction)
    {
        mActionMouseBindings.push_back(pAction);
    }

    void InputManager::Update()
    {
        if (mActionKeyBindings.empty() && mActionMouseBindings.empty())
        {
            return;
        }
        for (auto action : mActionKeyBindings)
        {
            if (!mContext->GetActiveScene()->GetSceneLoaded())
            {
                return;
            }
            auto& key = action.first;
            auto& actions = action.second;
            for (auto* action : actions)
            {
                action->Update();
                if (!mContext->GetActiveScene()->GetSceneLoaded())
                {
                    return;
                }
            }
        }
        for (InputActions* action : mActionMouseBindings)
        {
            action->Update();
            if (!mContext->GetActiveScene()->GetSceneLoaded())
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

    void InputManager::SetContext(ISceneContext* pContext)
    {
        mContext = pContext;
    }
}