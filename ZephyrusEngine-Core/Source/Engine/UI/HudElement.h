#pragma once

#include "Vector2D.h"
#include "Vector4D.h"
#include "RendererOpenGl.h"

class ISceneContext;

/**
 * @brief Base class for all HUD elements.
 * Provides position management and an interface for drawing HUD elements.
 */
namespace Zephyrus::UI {
    class HudElement
    {
    protected:
        Vector2D mPosition;
        float mDrawOrder{ 0 };
        ISceneContext* mContext{ nullptr };
    public:
        HudElement(ISceneContext* pContext);
        virtual ~HudElement() = default;
        virtual void Draw(Zephyrus::Render::IRenderer& pRenderer) = 0;

        // Sets the position of the HUD element.
        void SetPosition(const Vector2D& pPosition);

        inline Vector2D GetPosition() const { return mPosition; }

        void SetDrawOrder(float pDrawOrder);
        inline float GetDrawOrder() const { return mDrawOrder; }
    };
}