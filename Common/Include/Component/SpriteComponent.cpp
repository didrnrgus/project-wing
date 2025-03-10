#include "SpriteComponent.h"
#include "Shader/Shader.h"
#include "Shader/ShaderManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Mesh/MeshManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Scene/CameraManager.h"
#include "Shader/TransformCBuffer.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/SpriteCBuffer.h"

CSpriteComponent::CSpriteComponent()
{
    mRenderType = EComponentRender::Render;
    mRenderLayerName = "Object";
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& Com) :
    CSceneComponent(Com)
{
}

CSpriteComponent::CSpriteComponent(CSpriteComponent&& Com) :
    CSceneComponent(Com)
{
}

CSpriteComponent::~CSpriteComponent()
{
    SAFE_DELETE(mAnimation);
    SAFE_DELETE(mSpriteCBuffer);
}

void CSpriteComponent::SetShader(const std::string& Name)
{
    mShader = CShaderManager::GetInst()->FindShader(Name);
}

void CSpriteComponent::SetShader(CShader* Shader)
{
    mShader = Shader;
}

void CSpriteComponent::SetTexture(
    const std::string& Name,
    int TextureIndex)
{
    if (mScene)
        mTexture = mScene->GetAssetManager()->FindTexture(Name);

    else
        mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);

    mTextureIndex = TextureIndex;
}

void CSpriteComponent::SetTexture(
    const std::string& Name, const TCHAR* FileName,
    int TextureIndex)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
        {
            return;
        }
        mTexture = mScene->GetAssetManager()->FindTexture(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
        {
            return;
        }
        mTexture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
    }
    mTextureIndex = TextureIndex;
}

void CSpriteComponent::SetTexture(
    CTexture* Texture, 
    int TextureIndex)
{
    mTexture = Texture;
    mTextureIndex = TextureIndex;
}

void CSpriteComponent::SetTint(
    float r, float g, float b)
{
    mTint.x = r;
    mTint.y = g;
    mTint.z = b;
}

void CSpriteComponent::SetOpacity(float Opacity)
{
    mTint.w = Opacity;
}

void CSpriteComponent::SetColor(float r, float g, float b, float a)
{
    mTint.x = r;
    mTint.y = g;
    mTint.z = b;
    mTint.w = a;
}

void CSpriteComponent::SetColor(FVector4D color)
{
    mTint = color;
}

void CSpriteComponent::SetTextureIndex(int Index)
{
    mTextureIndex = Index;
}

bool CSpriteComponent::Init()
{
    CSceneComponent::Init();

    mSpriteCBuffer = new CSpriteCBuffer;

    mSpriteCBuffer->Init();

    SetShader("SpriteShader");

    if (mScene)
        mMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");

    else
        mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");

    return true;
}

bool CSpriteComponent::Init(const char* FileName)
{
    CSceneComponent::Init(FileName);

    mSpriteCBuffer = new CSpriteCBuffer;

    mSpriteCBuffer->Init();

    SetShader("SpriteShader");

    if (mScene)
        mMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");

    else
        mMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");

    return true;
}

void CSpriteComponent::PreUpdate(float DeltaTime)
{
    CSceneComponent::PreUpdate(DeltaTime);
}

void CSpriteComponent::Update(float DeltaTime)
{
    CSceneComponent::Update(DeltaTime);

    if (mAnimation)
        mAnimation->Update(DeltaTime);
}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
    CSceneComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::Collision(float DeltaTime)
{
    CSceneComponent::Collision(DeltaTime);
}

void CSpriteComponent::PreRender()
{
    CSceneComponent::PreRender();
}

void CSpriteComponent::Render()
{
    CSceneComponent::Render();

    if (mAnimation)
    {
        mAnimation->SetShader();
    }

    else
    {
        CAnimation2D::DisableAnimation();
    }

    mSpriteCBuffer->SetTint(mTint);

    mSpriteCBuffer->UpdateBuffer();

    mTransformCBuffer->SetWorldMatrix(mmatWorld);

    FMatrix matView, matProj;
    matView = mScene->GetCameraManager()->GetViewMatrix();
    matProj = mScene->GetCameraManager()->GetProjMatrix();

    mTransformCBuffer->SetViewMatrix(matView);
    mTransformCBuffer->SetProjMatrix(matProj);
    mTransformCBuffer->SetPivot(mPivot);

    mTransformCBuffer->UpdateBuffer();

    mShader->SetShader();

    if (mTexture)
    {
        mTexture->SetShader(0, EShaderBufferType::Pixel,
            mTextureIndex);
    }

    mMesh->Render();
}

void CSpriteComponent::PostRender()
{
    CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
    return new CSpriteComponent(*this);
}

