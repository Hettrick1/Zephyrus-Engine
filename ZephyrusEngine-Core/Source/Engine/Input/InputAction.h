#pragma once
#include "ActionType.h"
#include <functional>
#include "Vector2D.h"
#include <vector>

namespace Zephyrus::Inputs {
	class InputAction
	{
	protected:
		std::string mName;
		ActionType mType;
		std::vector<int> mKeys;
		std::vector<int> mMouseButtons;
		
	public:
		bool IsDown = false;
		bool IsMouseAxis = false;
		
	public:
		InputAction(const std::string& pName, ActionType type);
		virtual ~InputAction() = default;
		
		void SetName(const std::string& pName);
		
		const std::string& GetName() const { return mName; }
		ActionType GetType() const { return mType; }
		
		void BindKey(int key) { mKeys.push_back(key); }
		bool IsBoundToKey(int key) const;

		void BindMouseButton(int button) { mMouseButtons.push_back(button); }
		bool IsBoundToMouse(int button) const;
	};
}