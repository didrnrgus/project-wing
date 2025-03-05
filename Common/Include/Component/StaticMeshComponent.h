#pragma once

#include "MeshComponent.h"

class CStaticMeshComponent :
    public CMeshComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& Com);
	CStaticMeshComponent(CStaticMeshComponent&& Com);
	virtual ~CStaticMeshComponent();

protected:
	CSharedPtr<class CStaticMesh>	mMesh;
	CSharedPtr<class CShader>		mShader;

public:
	void SetShader(const std::string& Name);
	void SetShader(class CShader* Shader);
	void SetMesh(const std::string& Name);
	void SetMesh(class CMesh* Mesh);
	void SetMaterial(int SlotIndex, const std::string& Name);
	void SetMaterial(int SlotIndex, class CMaterial* Material);
	void AddTexture(int SlotIndex, const std::string& Name,
		int Register, int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);
	void AddTexture(int SlotIndex, const std::string& Name,
		const TCHAR* FileName, int Register,
		int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);
	void AddTexture(int SlotIndex, class CTexture* Texture,
		int Register,
		int ShaderBufferType = EShaderBufferType::Pixel,
		int TextureIndex = 0);
	void SetBaseColor(int SlotIndex, float r, float g,
		float b, float a);
	void SetOpacity(int SlotIndex, float Opacity);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CStaticMeshComponent* Clone();
};

