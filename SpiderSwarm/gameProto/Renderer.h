#pragma once

#include "Math.h"
#include "VertexArray.h"

#include <string>
#include <unordered_map>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Effekseer.h>
#include <EffekseerRendererGL.h>

class Shader;
class Texture;
class Mesh;
class MeshComponent;
class Skeleton;
class SkeletalMeshComponent;
class Animation;
class DepthMap;
class HDRRenderer;
class CubeMap;
class VertexArray;
class PostEffect;
class EffekseerEffect;
class InstanceMeshComponent;
class InstanceMeshManager;
class GBuffer;
class PointLightComponent;
class AttachMeshComponent;

typedef struct _DirectionalLight
{
	Vector3 mDirection;     // ���C�g����
	Vector3 mDiffuseColor;	// ���C�g�J���[
	Vector3 mSpecColor;	    // �X�y�L�����[�J���[
}DirectionalLight;


enum class TextureStage
{
	DiffuseMap,
	NormalMap,
	SpecularMap,
	EmissiveMap,
	ShadowMap,
};


class Renderer
{
public:
	Renderer();
	~Renderer();
	bool                   Initialize(int screenWidth, int screenHeight, bool fullScreen);     // SDL & GL������
	void                   Shutdown();                                                         // �I��
	void                   Draw();                                                             // �`��

	// �Z�b�^�[�n
	void                   SetViewMatrix(const Matrix4& view);
	void                   SetProjMatrix(const Matrix4& proj);          // �v���W�F�N�V�����s��ɃZ�b�g
	void                   SetAmbientLight(const Vector3& ambientColor)                        // �A���r�G���g���C�g�̃Z�b�g
	{
		mAmbientLight = ambientColor;
	}
	// �Q�b�^�[�n
	SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }                           // SDL�n�̕`��ɕK�v��SDLrenderer�𓾂�
	Texture* GetTexture(const std::string& fileName);                            // �e�N�X�`�����t�@�C��������Ԃ�
	Mesh* GetMesh(const std::string& fileName);                               // ���b�V�����t�@�C��������Ԃ�
	const Skeleton* GetSkeleton(const char* fileName);                                  // �X�P���^�����f���̎擾
	const Animation* GetAnimation(const char* fileName, bool loop);                      // �X�P���^���A�j���[�V�����̎擾
	EffekseerEffect* GetEffect(const char16_t* fileName);
	InstanceMeshManager* GetInstanceMeshManager() { return mInstanceMeshManager; }

	float GetScreenWidth() { return static_cast<float>(mScreenWidth); }       // �X�N���[����
	float GetScreenHeight() { return static_cast<float>(mScreenHeight); }     // �X�N���[������
	DirectionalLight& GetDirectionalLight() { return mDirectionalLight; }                 // �f�B���N�V���i�����C�g
	const Matrix4& GetViewMatrix() { return mView; }
	const Matrix4& GetProjectionMatrix() { return mProjection; }
	unsigned int GetUndefineTexID() { return mUndefineTexID; }

	void AddMeshComponent(MeshComponent* mesh);    // ���b�V���R���|�[�l���g�̒ǉ�
	void RemoveMeshComponent(MeshComponent* mesh); // ���b�V���R���|�[�l���g�̍폜
	void AddInstanceMeshComponent(InstanceMeshComponent* instanceMesh);
	void RemoveInstanceMeshComponent(InstanceMeshComponent* instanMesh);
	void AddAttachMeshComponent(AttachMeshComponent* attachMeshComp);
	void RemoveAttachMeshComponent(AttachMeshComponent* attachMeshComp);
	void AddPointLight(PointLightComponent* light);
	void RemovePointLight(PointLightComponent* light);
	void ShowResource();                                                     // �o�^����Ă��� �e�N�X�`���E���b�V�����\�[�X�̕\���i�f�o�b�O�p�j
	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } // �E�B���h�E�`��N���A
	void WindowFlip() { SDL_GL_SwapWindow(mWindow); }                        // �E�B���h�E�t���b�v
	void SetWindowTitle(const std::string& title);                           // �E�B���h�E�^�C�g���̃Z�b�g

	void SetDepthSetting(const Vector3& centerWorldPos,
		const Vector3& lightDir,
		const Vector3& upVec,
		float          lightDistance);

	void SpriteDrawBegin();
	void SpriteDrawEnd();

	void                   DrawTexture(Texture* texture,
		int index, int xDivNum, int yDivNum,
		const Vector2& offset,
		float scale = 1.0f, float alpha = 1.0f);
	void                   DrawTexture(Texture* texture, const Vector2& offset,
		float scale = 1.0f, float alpha = 1.0f);

	// Effekseer�֘A
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return mEffekseerRenderer; }
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager() { return mEffekseerManager; }

private:
	bool LoadShaders();                          // �V�F�[�_�[�̏�����
	void SetLightUniforms(class Shader* shader,const Matrix4& view); // ���C�g�l���V�F�[�_�[�ɃZ�b�g
	void CreateSpriteVerts();                    // �X�v���C�g���_�쐬

	bool CreateMirrorTarget();
	void Draw3DScene(unsigned int frameBuffer, const Matrix4& view, const Matrix4& proj,float viewPortScale=1.0f,bool lit=true);
	void DrawFromGBuffer();

	int mScreenWidth;      // �X�N���[����                                                           
	int mScreenHeight;     // �X�N���[������
	std::unordered_map<std::string, Texture*>   mTextures;			// �e�N�X�`���o�^�z��
	std::unordered_map<std::string, Mesh*>      mMeshs;				// ���b�V���o�^�z��
	std::vector<MeshComponent*>                 mMeshComponents;	// ���b�V���R���|�[�l���g�o�^�z��
	std::vector<InstanceMeshComponent*>			mInstanceMeshes;	// �C���X�^���X���b�V���R���|�[�l���g�̓o�^�z��
	std::vector<SkeletalMeshComponent*>         mSkeletalMeshes;	// �X�P���^�����b�V���̕`��Ɏg����
	std::vector<AttachMeshComponent*>			mAttachMeshs;
	std::unordered_map<std::string, Skeleton*>  mSkeletons;			// �X�P���^���f�[�^
	std::unordered_map<std::string, Animation*> mAnims;				// �A�j���[�V�����f�[�^
	std::unordered_map<const char16_t*, EffekseerEffect*> mEffects;


	//�V�F�[�_�[�֘A
	Shader* mMeshShader;            // ���b�V���V�F�[�_�[
	Shader* mMeshShadowHDRShader;   // �ʏ탁�b�V���V���h�E�V�F�[�_�[
	Shader* mSkinnedShader;         // �X�L�����b�V���V�F�[�_�[
	Shader* mSkinnedDepthShader;    // �X�L���f�v�X�p�V�F�[�_�[ 
	Shader* mSkinnedShadowHDRShader;// �X�L���V���h�E�V�F�[�_�[
	Shader* mSpriteShader;          // �X�v���C�g�V�F�[�_�[
	Shader* mTilemapShader;         // �^�C���}�b�v�V�F�[�_
	Shader* mInstanceShader;		// �C���X�^���X�p�V�F�[�_�[
	Shader* mUIShader;				// UI�p�V�F�[�_�[

	DepthMap* mDepthMapRender;		// �f�v�X�}�b�v�����_���[
	HDRRenderer* mHDRRenderer;      // HDR �����_���[
	CubeMap* mCubeMap;				// �L���[�u�}�b�v
	PostEffect* mPostEffect;		// �|�X�g�G�t�F�N�g

	// ��{�s��֘A
	Matrix4                                           mView;             // �r���[�s��
	Matrix4                                           mProjection;       // �v���W�F�N�V�����s��

	// �X�v���C�g���_�z��
	VertexArray* mSpriteVerts;

	// ���C�e�B���O�֘A
	Vector3                                           mAmbientLight;     // �A���r�G���g���C�g
	DirectionalLight                                  mDirectionalLight; // �f�B���N�V���i�����C�g

	// �����_�����O�x�[�X���֘A
	SDL_Window* mWindow;           // SDL�E�B���h�E�n���h�� 
	SDL_GLContext                                     mContext;          // OpenGL�R���e�L�X�g
	SDL_Renderer* mSDLRenderer;      // SDL�����_�����O�n���h��

	unsigned int mUndefineTexID;    // ���ݒ�e�N�X�`���̏ꍇ�Ɋ��蓖�Ă��鍕�F�e�N�X�`��

	// Effekseer�֘A
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> mEffekseerRenderer; // Effekseer�����_��
	Effekseer::RefPtr<Effekseer::Manager>            mEffekseerManager; // Effekseer�}�l�[�W��  
	
	InstanceMeshManager* mInstanceMeshManager;						// �C���X�^���X�}�l�[�W���[

	// �W�I���g���C���X�^���X
	//std::unordered_map<InstanceType, GameObject*>;

	// G�o�b�t�@�֘A
	GBuffer* mGBuffer;
	Shader* mGGlobalShader;
	Shader* mGPointLightShader;
	std::vector<PointLightComponent*> mPointLights;
	Mesh* mPointLightMesh;

	unsigned int mMirrorBuffer;
	Texture* mMirrorTexture;
	Matrix4 mMirrorView;
};

bool GLErrorHandle(const char* location);                              // OpenGL�̃G���[�n���h���擾