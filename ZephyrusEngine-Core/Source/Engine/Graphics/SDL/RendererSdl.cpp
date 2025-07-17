#include "RendererSdl.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Maths.h"
#include "Actor.h"
#include "Texture.h"

RendererSdl::RendererSdl() 
    : mSdlRenderer(nullptr)
{

}

bool RendererSdl::Initialize(Window& rWindow)
{
    mSdlRenderer = SDL_CreateRenderer(rWindow.GetSdlWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mSdlRenderer)
    {
        Log::Error(LogType::Video, "Failed to create Renderer");
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) 
    {
        Log::Error(LogType::Video, "Unable to initialize SDL_Image");
        return false;
    }
    return true;
}

void RendererSdl::BeginDraw()
{
    SDL_SetRenderDrawColor(mSdlRenderer, 120, 120, 255, 255);
    SDL_RenderClear(mSdlRenderer);
}

void RendererSdl::Draw()
{
    for (SpriteComponent* sprite : mSprites) {
        sprite->Draw(*this);
    }
}

void RendererSdl::EndDraw()
{
    SDL_RenderPresent(mSdlRenderer);
}

void RendererSdl::Close()
{
    SDL_DestroyRenderer(mSdlRenderer);
}

void RendererSdl::Unload()
{
}

void RendererSdl::AddSprite(SpriteComponent* pSprite)
{
    int spriteDrawOrder = pSprite->GetDrawOrder();
    std::vector<SpriteComponent*>::iterator sc;
    for (sc = mSprites.begin(); sc != mSprites.end(); ++sc) 
    {
        if (spriteDrawOrder < (*sc)->GetDrawOrder()) break;  
    } 
    mSprites.insert(sc, pSprite);
}

void RendererSdl::RemoveSprite(SpriteComponent* pSprite)
{
    std::vector<SpriteComponent*>::iterator sc;
    sc = std::find(mSprites.begin(), mSprites.end(), pSprite);
    mSprites.erase(sc);
}

void RendererSdl::AddSkySphere(SkySphereComponent* pSkySphere)
{
}

void RendererSdl::RemoveSkySphere()
{
}

void RendererSdl::DrawRect(Rectangle& rRect)
{
    SDL_SetRenderDrawColor(mSdlRenderer, 255, 255, 255, 255);
    SDL_Rect sdlRect = rRect.ToSdlRect();
    SDL_RenderFillRect(mSdlRenderer, &sdlRect);
}

void RendererSdl::DrawSprite(Actor& pActor, Texture& pTexture, Rectangle pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const
{
    SDL_Rect destinationRect; 
    TransformComponent& transform = pActor.GetTransformComponent();
    destinationRect.w = static_cast<int>(pTexture.GetTextureSize().x * transform.GetSize().x);
    destinationRect.h = static_cast<int>(pTexture.GetTextureSize().y * transform.GetSize().y);
    destinationRect.x = static_cast<int>(transform.GetPosition().x - pOrigin.x); 
    destinationRect.y = static_cast<int>(transform.GetPosition().y - pOrigin.y); 

    SDL_Rect* sourceSDL = nullptr; 
    if (pRect != Rectangle::Null) 
    {
        sourceSDL = new SDL_Rect{ 
            Maths::Round(pRect.position.x),
            Maths::Round(pRect.position.y),
            Maths::Round(pRect.dimensions.x),
            Maths::Round(pRect.dimensions.y) };
    }
    SDL_RendererFlip flip;
    switch (pFlipMethod)
    {
    case RendererSdl::Flip::None:
        flip = SDL_FLIP_NONE;
        break;
    case RendererSdl::Flip::Horizontal:
        flip = SDL_FLIP_HORIZONTAL;
        break;
    case RendererSdl::Flip::Vertical:
        flip = SDL_FLIP_VERTICAL;
        break;
    default:
        flip = SDL_FLIP_NONE;
        break;
    }
    SDL_RenderCopyEx(mSdlRenderer, pTexture.GetSdlTexture(), sourceSDL, &destinationRect, -Maths::ToDeg(transform.GetRotation().z), nullptr, flip); 

    delete sourceSDL;

}

SDL_Renderer* RendererSdl::ToSdlRenderer()
{
    return mSdlRenderer;
}
