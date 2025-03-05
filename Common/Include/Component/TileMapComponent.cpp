#include "TileMapComponent.h"
#include "TileMapRendererComponent.h"
#include "../Object/SceneObject.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Mesh/MeshManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/ColliderCBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/TileMapCBuffer.h"
#include "Shader/Shader.h"
#include "Shader/ShaderManager.h"
#include "../Device.h"
#include "Asset/Texture/Texture.h"

CTileMapComponent::CTileMapComponent()
{
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& Com)  :
    CComponent(Com)
{
}

CTileMapComponent::CTileMapComponent(CTileMapComponent&& Com) :
    CComponent(Com)
{
}

CTileMapComponent::~CTileMapComponent()
{
    size_t  Size = mTileList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        SAFE_DELETE(mTileList[i]);
    }

    SAFE_DELETE(mTileMapCBuffer);
    SAFE_DELETE(mColorCBuffer);
    SAFE_DELETE(mLineTransformCBuffer);
}

void CTileMapComponent::SetTileOutLineRender(bool Render)
{
    mTileOutLineRender = Render;

    if (mTileOutLineRender)
    {
        mOutLineShader = CShaderManager::GetInst()->FindShader("FrameMeshShader");

        if (mScene)
            mOutLineMesh = mScene->GetAssetManager()->FindMesh("FrameLBRect");

        else
            mOutLineMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("FrameLBRect");
    }

    else
    {
        mOutLineMesh = nullptr;
        mOutLineShader = nullptr;
    }
}

void CTileMapComponent::SetTileTexture(
    const std::string& Name)
{
    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
        Renderer->SetTileTexture(Name);
}

void CTileMapComponent::SetTileTexture(
    const std::string& Name, const TCHAR* FileName)
{
    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
        Renderer->SetTileTexture(Name, FileName);
}

void CTileMapComponent::SetTileTexture(CTexture* Texture)
{
    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
        Renderer->SetTileTexture(Texture);
}

void CTileMapComponent::AddTileTextureFrame(
    const FVector2D& Start, const FVector2D& Size)
{
    FAnimationFrame Frame;
    Frame.Start = Start;
    Frame.Size = Size;

    mTileFrameList.emplace_back(Frame);
}

void CTileMapComponent::AddTileTextureFrame(float StartX,
    float StartY, float SizeX, float SizeY)
{
    FAnimationFrame Frame;
    Frame.Start.x = StartX;
    Frame.Start.y = StartY;
    Frame.Size.x = SizeX;
    Frame.Size.y = SizeY;

    mTileFrameList.emplace_back(Frame);
}

int CTileMapComponent::GetTileIndexX(const FVector3D& Pos) const
{
    float Convert = Pos.x -
        mOwnerObject->GetWorldPosition().x;

    float   Value = Convert / mTileSize.x;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountX) ? -1 : Index;
}

int CTileMapComponent::GetTileIndexX(const FVector2D& Pos) const
{
    float Convert = Pos.x -
        mOwnerObject->GetWorldPosition().x;

    float   Value = Convert / mTileSize.x;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountX) ? -1 : Index;
}

int CTileMapComponent::GetTileIndexX(float x) const
{
    float Convert = x - mOwnerObject->GetWorldPosition().x;

    float   Value = Convert / mTileSize.x;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountX) ? -1 : Index;
}

int CTileMapComponent::GetTileIndexY(const FVector3D& Pos) const
{
    float Convert = Pos.y -
        mOwnerObject->GetWorldPosition().y;

    float   Value = Convert / mTileSize.y;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountY) ? -1 : Index;
}

int CTileMapComponent::GetTileIndexY(const FVector2D& Pos) const
{
    float Convert = Pos.y -
        mOwnerObject->GetWorldPosition().y;

    float   Value = Convert / mTileSize.y;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountY) ? -1 : Index;
}

int CTileMapComponent::GetTileIndexY(float y) const
{
    float Convert = y - mOwnerObject->GetWorldPosition().y;

    float   Value = Convert / mTileSize.y;

    if (Value < 0.f)
        return -1;

    int Index = (int)Value;

    return (Index < 0 || Index >= mCountY) ? -1 : Index;
}

int CTileMapComponent::GetTileIndex(const FVector3D& Pos) const
{
    int IndexX = GetTileIndexX(Pos.x);

    if (IndexX == -1)
        return -1;

    int IndexY = GetTileIndexY(Pos.y);

    if (IndexY == -1)
        return -1;

    return IndexY * mCountX + IndexX;
}

int CTileMapComponent::GetTileIndex(const FVector2D& Pos) const
{
    int IndexX = GetTileIndexX(Pos.x);

    if (IndexX == -1)
        return -1;

    int IndexY = GetTileIndexY(Pos.y);

    if (IndexY == -1)
        return -1;

    return IndexY * mCountX + IndexX;
}

int CTileMapComponent::GetTileIndex(float x, float y) const
{
    int IndexX = GetTileIndexX(x);

    if (IndexX == -1)
        return -1;

    int IndexY = GetTileIndexY(y);

    if (IndexY == -1)
        return -1;

    return IndexY * mCountX + IndexX;
}

const CTile* CTileMapComponent::GetTile(const FVector3D& Pos) const
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return nullptr;

    return mTileList[Index];
}

const CTile* CTileMapComponent::GetTile(const FVector2D& Pos) const
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return nullptr;

    return mTileList[Index];
}

const CTile* CTileMapComponent::GetTile(float x, float y) const
{
    int Index = GetTileIndex(x, y);

    if (Index == -1)
        return nullptr;

    return mTileList[Index];
}

ETileType CTileMapComponent::ChangeTileType(ETileType Type,
    const FVector3D& Pos)
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return ETileType::None;

    ETileType   PrevType = mTileList[Index]->GetType();

    mTileList[Index]->SetTileType(Type);

    return PrevType;
}

ETileType CTileMapComponent::ChangeTileType(ETileType Type,
    const FVector2D& Pos)
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return ETileType::None;

    ETileType   PrevType = mTileList[Index]->GetType();

    mTileList[Index]->SetTileType(Type);

    return PrevType;
}

ETileType CTileMapComponent::ChangeTileType(ETileType Type,
    float x, float y)
{
    int Index = GetTileIndex(x, y);

    if (Index == -1)
        return ETileType::None;

    ETileType   PrevType = mTileList[Index]->GetType();

    mTileList[Index]->SetTileType(Type);

    return PrevType;
}

ETileType CTileMapComponent::ChangeTileType(ETileType Type, 
    int Index)
{
    ETileType   PrevType = mTileList[Index]->GetType();

    mTileList[Index]->SetTileType(Type);

    return PrevType;
}

void CTileMapComponent::ChangeTileFrame(int Frame, 
    const FVector3D& Pos)
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return;

    mTileList[Index]->SetTextureFrame(Frame);
}

void CTileMapComponent::ChangeTileFrame(int Frame, 
    const FVector2D& Pos)
{
    int Index = GetTileIndex(Pos);

    if (Index == -1)
        return;

    mTileList[Index]->SetTextureFrame(Frame);
}

void CTileMapComponent::ChangeTileFrame(int Frame, 
    float x, float y)
{
    int Index = GetTileIndex(x, y);

    if (Index == -1)
        return;

    mTileList[Index]->SetTextureFrame(Frame);
}

void CTileMapComponent::ChangeTileFrame(int Frame, int Index)
{
    if (Index == -1)
        return;

    mTileList[Index]->SetTextureFrame(Frame);
}

bool CTileMapComponent::Init()
{
    CComponent::Init();

    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
        Renderer->SetTileMapComponent(this);

    mLineTransformCBuffer = new CTransformCBuffer;

    mLineTransformCBuffer->Init();

    mColorCBuffer = new CColliderCBuffer;

    mColorCBuffer->Init();

    mTileMapCBuffer = new CTileMapCBuffer;

    mTileMapCBuffer->Init();


    mTileShader = CShaderManager::GetInst()->FindShader("TileShader");

    if (mScene)
        mTileMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");

    else
        mTileMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");

    return true;
}

bool CTileMapComponent::Init(const char* FileName)
{
    CComponent::Init(FileName);

    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
        Renderer->SetTileMapComponent(this);

    mLineTransformCBuffer = new CTransformCBuffer;

    mLineTransformCBuffer->Init();

    mColorCBuffer = new CColliderCBuffer;

    mColorCBuffer->Init();

    mTileMapCBuffer = new CTileMapCBuffer;

    mTileMapCBuffer->Init();


    mTileShader = CShaderManager::GetInst()->FindShader("TileShader");

    if (mScene)
        mTileMesh = mScene->GetAssetManager()->FindMesh("SpriteRect");

    else
        mTileMesh = CAssetManager::GetInst()->GetMeshManager()->FindMesh("SpriteRect");

    return true;
}

void CTileMapComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);
}

void CTileMapComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);

    // 화면의 Min, Max를 구한다.
    FVector3D   Center = mScene->GetCameraManager()->GetCameraWorldPos();

    // 타일맵의 시작점 좌표를 빼준다.
    Center -= mOwnerObject->GetWorldPosition();

    FResolution RS = CDevice::GetInst()->GetResolution();

    mViewStartX = (int)((Center.x - RS.Width * 0.5f) / mTileSize.x);
    mViewStartY = (int)((Center.y - RS.Height * 0.5f) / mTileSize.y);

    mViewEndX = (int)((Center.x + RS.Width * 0.5f) / mTileSize.x);
    mViewEndY = (int)((Center.y + RS.Height * 0.5f) / mTileSize.y);

    if (mViewStartX < 0)
        mViewStartX = 0;

    else if (mViewStartX >= mCountX)
        mViewStartX = mCountX - 1;

    if (mViewEndX < 0)
        mViewEndX = 0;

    else if (mViewEndX >= mCountX)
        mViewEndX = mCountX - 1;

    if (mViewStartY < 0)
        mViewStartY = 0;

    else if (mViewStartY >= mCountY)
        mViewStartY = mCountY - 1;

    if (mViewEndY < 0)
        mViewEndY = 0;

    else if (mViewEndY >= mCountY)
        mViewEndY = mCountY - 1;
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);
}

CTileMapComponent* CTileMapComponent::Clone()
{
    return new CTileMapComponent(*this);
}

void CTileMapComponent::EndFrame()
{
    CComponent::EndFrame();
}

void CTileMapComponent::RenderTile()
{
    for (int i = mViewStartY; i <= mViewEndY; ++i)
    {
        for (int j = mViewStartX; j <= mViewEndX; ++j)
        {
            int Index = i * mCountX + j;

            int TileFrame = mTileList[Index]->GetTextureFrame();

            if (TileFrame == -1)
                continue;

            FMatrix  matScale, matTranslate, matWorld;

            matScale.Scaling(mTileList[Index]->GetSize());

            FVector2D   Pos = mTileList[Index]->GetPos();

            Pos.x += mOwnerObject->GetWorldPosition().x;
            Pos.y += mOwnerObject->GetWorldPosition().y;

            matTranslate.Translation(Pos);

            matWorld = matScale * matTranslate;

            mLineTransformCBuffer->SetWorldMatrix(matWorld);
            mLineTransformCBuffer->SetViewMatrix(mScene->GetCameraManager()->GetViewMatrix());
            mLineTransformCBuffer->SetProjMatrix(mScene->GetCameraManager()->GetProjMatrix());

            mLineTransformCBuffer->UpdateBuffer();

            FVector2D   LTUV, RBUV;

            LTUV = mTileFrameList[TileFrame].Start /
                mTileTextureSize;
            RBUV = LTUV + mTileFrameList[TileFrame].Size /
                mTileTextureSize;

            mTileMapCBuffer->SetUV(LTUV, RBUV);

            mTileMapCBuffer->UpdateBuffer();

            mTileShader->SetShader();
            
            mTileMesh->Render();
        }
    }
}

void CTileMapComponent::RenderTileOutLine()
{
    for (int i = mViewStartY; i <= mViewEndY; ++i)
    {
        for (int j = mViewStartX; j <= mViewEndX; ++j)
        {
            int Index = i * mCountX + j;

            FMatrix  matScale, matTranslate, matWorld;

            matScale.Scaling(mTileList[Index]->GetSize());

            FVector2D   Pos = mTileList[Index]->GetPos();

            Pos.x += mOwnerObject->GetWorldPosition().x;
            Pos.y += mOwnerObject->GetWorldPosition().y;

            matTranslate.Translation(Pos);

            matWorld = matScale * matTranslate;

            mLineTransformCBuffer->SetWorldMatrix(matWorld);
            mLineTransformCBuffer->SetViewMatrix(mScene->GetCameraManager()->GetViewMatrix());
            mLineTransformCBuffer->SetProjMatrix(mScene->GetCameraManager()->GetProjMatrix());

            mLineTransformCBuffer->UpdateBuffer();

            switch (mTileList[Index]->GetType())
            {
            case ETileType::Normal:
                mColorCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);
                break;
            case ETileType::UnableToMove:
                mColorCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
                break;
            case ETileType::MouseOver:
                mColorCBuffer->SetColor(1.f, 1.f, 0.f, 1.f);
                break;
            }

            mColorCBuffer->UpdateBuffer();

            mOutLineShader->SetShader();

            mOutLineMesh->Render();
        }
    }
}

void CTileMapComponent::CreateTile(ETileShape Shape,
    int CountX, int CountY,
    const FVector2D& TileSize, int TileTextureFrame)
{
    mTileShape = Shape;
    mCountX = CountX;
    mCountY = CountY;
    mTileSize = TileSize;

    mTileList.resize(mCountX * mCountY);

    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
    {
        FVector2D   Scale;
        Scale.x = mTileSize.x * mCountX;
        Scale.y = mTileSize.y * mCountY;
        Renderer->SetWorldScale(Scale);
    }

    for (int i = 0; i < mCountY; ++i)
    {
        for (int j = 0; j < mCountX; ++j)
        {
            CTile* Tile = new CTile;

            Tile->mPos.x = j * mTileSize.x;
            Tile->mPos.y = i * mTileSize.y;
            Tile->mSize = mTileSize;
            Tile->mCenter = Tile->mPos + mTileSize * 0.5f;
            Tile->mTextureFrame = TileTextureFrame;

            mTileList[i * mCountX + j] = Tile;
        }
    }
}

void CTileMapComponent::Save(const char* FileName)
{
    char	FullPath[MAX_PATH] = {};

    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "..\\Asset\\Data\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;

    fopen_s(&File, FullPath, "wb");

    if (!File)
        return;

    fwrite(&mTileShape, sizeof(ETileShape), 1, File);
    fwrite(&mTileSize, sizeof(FVector2D), 1, File);
    fwrite(&mCountX, sizeof(int), 1, File);
    fwrite(&mCountY, sizeof(int), 1, File);

    int FrameCount = (int)mTileFrameList.size();

    fwrite(&FrameCount, sizeof(int), 1, File);

    fwrite(&mTileFrameList[0], sizeof(FAnimationFrame),
        FrameCount, File);

    fwrite(&mTileTextureSize, sizeof(FVector2D), 1, File);

    int TileCount = (int)mTileList.size();

    for (int i = 0; i < TileCount; ++i)
    {
        mTileList[i]->Save(File);
    }

    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
    {
        bool    TextureEnable = false;

        CTexture* Texture = Renderer->GetBackTexture();

        if (Texture)
        {
            TextureEnable = true;

            fwrite(&TextureEnable, sizeof(bool), 1, File);

            int NameCount = (int)Texture->GetName().length();

            fwrite(&NameCount, sizeof(int), 1, File);

            fwrite(Texture->GetName().c_str(), sizeof(char),
                NameCount, File);

            fwrite(Texture->GetTexture(0)->FileName,
                sizeof(TCHAR), MAX_PATH, File);
        }

        else
            fwrite(&TextureEnable, sizeof(bool), 1, File);

        TextureEnable = false;
        Texture = Renderer->GetTileTexture();

        if (Texture)
        {
            TextureEnable = true;

            fwrite(&TextureEnable, sizeof(bool), 1, File);

            int NameCount = (int)Texture->GetName().length();

            fwrite(&NameCount, sizeof(int), 1, File);

            fwrite(Texture->GetName().c_str(), sizeof(char),
                NameCount, File);

            fwrite(Texture->GetTexture(0)->FileName,
                sizeof(TCHAR), MAX_PATH, File);
        }

        else
            fwrite(&TextureEnable, sizeof(bool), 1, File);

    }

    fclose(File);
}

void CTileMapComponent::Load(const char* FileName)
{
    char	FullPath[MAX_PATH] = {};

    strcpy_s(FullPath, gRootPathMultibyte);
    strcat_s(FullPath, "..\\Asset\\Data\\");
    strcat_s(FullPath, FileName);

    FILE* File = nullptr;

    fopen_s(&File, FullPath, "rb");

    if (!File)
        return;

    fread(&mTileShape, sizeof(ETileShape), 1, File);
    fread(&mTileSize, sizeof(FVector2D), 1, File);
    fread(&mCountX, sizeof(int), 1, File);
    fread(&mCountY, sizeof(int), 1, File);

    int FrameCount = 0;

    fread(&FrameCount, sizeof(int), 1, File);

    mTileFrameList.clear();
    mTileFrameList.resize((size_t)FrameCount);

    fread(&mTileFrameList[0], sizeof(FAnimationFrame),
        FrameCount, File);

    fread(&mTileTextureSize, sizeof(FVector2D), 1, File);

    size_t  Size = mTileList.size();

    for (size_t i = 0; i < Size; ++i)
    {
        SAFE_DELETE(mTileList[i]);
    }

    mTileList.clear();

    int TileCount = mCountX * mCountY;

    mTileList.resize((size_t)TileCount);

    for (int i = 0; i < TileCount; ++i)
    {
        CTile* Tile = new CTile;

        Tile->Load(File);

        mTileList[i] = Tile;
    }

    CTileMapRendererComponent* Renderer =
        mOwnerObject->FindSceneComponent<CTileMapRendererComponent>();

    if (Renderer)
    {
        bool    TextureEnable = false;

        fread(&TextureEnable, sizeof(bool), 1, File);

        if (TextureEnable)
        {
            char    TexName[256] = {};

            int NameCount = 0;

            fread(&NameCount, sizeof(int), 1, File);
            fread(TexName, sizeof(char), NameCount, File);

            TCHAR   FileName[MAX_PATH] = {};

            fread(FileName,
                sizeof(TCHAR), MAX_PATH, File);

            Renderer->SetBackTexture(TexName, FileName);
        }

        fread(&TextureEnable, sizeof(bool), 1, File);

        if (TextureEnable)
        {
            char    TexName[256] = {};

            int NameCount = 0;

            fread(&NameCount, sizeof(int), 1, File);
            fread(TexName, sizeof(char), NameCount, File);

            TCHAR   FileName[MAX_PATH] = {};

            fread(FileName,
                sizeof(TCHAR), MAX_PATH, File);

            Renderer->SetTileTexture(TexName, FileName);
        }
    }

    fclose(File);
}
