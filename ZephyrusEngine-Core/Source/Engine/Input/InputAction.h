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
		
	public:
		bool IsDown = false;
		bool IsMouseAxis = false;
		
	public:
		InputAction(const std::string& pName, ActionType type);
		virtual ~InputAction() = default;
		
		void SetName(const std::string& pName);
		
		const std::string& GetName() const { return mName; }
		ActionType GetType() const { return mType; }
		
		virtual bool IsBoundToKey(int key) const = 0;
		virtual bool IsBoundToMouse(int button) const = 0;
	};
}