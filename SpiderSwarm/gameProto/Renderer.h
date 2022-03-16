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
	Vector3 mDirection;     // ライト方向
	Vector3 mDiffuseColor;	// ライトカラー
	Vector3 mSpecColor;	    // スペキュラーカラー
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
	bool                   Initialize(int screenWidth, int screenHeight, bool fullScreen);     // SDL & GL初期化
	void                   Shutdown();                                                         // 終了
	void                   Draw();                                                             // 描画

	// セッター系
	void                   SetViewMatrix(const Matrix4& view);
	void                   SetProjMatrix(const Matrix4& proj);          // プロジェクション行列にセット
	void                   SetAmbientLight(const Vector3& ambientColor)                        // アンビエントライトのセット
	{
		mAmbientLight = ambientColor;
	}
	// ゲッター系
	SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }                           // SDL系の描画に必要なSDLrendererを得る
	Texture* GetTexture(const std::string& fileName);                            // テクスチャをファイル名から返す
	Mesh* GetMesh(const std::string& fileName);                               // メッシュをファイル名から返す
	const Skeleton* GetSkeleton(const char* fileName);                                  // スケルタルモデルの取得
	const Animation* GetAnimation(const char* fileName, bool loop);                      // スケルタルアニメーションの取得
	EffekseerEffect* GetEffect(const char16_t* fileName);
	InstanceMeshManager* GetInstanceMeshManager() { return mInstanceMeshManager; }

	float GetScreenWidth() { return static_cast<float>(mScreenWidth); }       // スクリーン幅
	float GetScreenHeight() { return static_cast<float>(mScreenHeight); }     // スクリーン高さ
	DirectionalLight& GetDirectionalLight() { return mDirectionalLight; }                 // ディレクショナルライト
	const Matrix4& GetViewMatrix() { return mView; }
	const Matrix4& GetProjectionMatrix() { return mProjection; }
	unsigned int GetUndefineTexID() { return mUndefineTexID; }

	void AddMeshComponent(MeshComponent* mesh);    // メッシュコンポーネントの追加
	void RemoveMeshComponent(MeshComponent* mesh); // メッシュコンポーネントの削除
	void AddInstanceMeshComponent(InstanceMeshComponent* instanceMesh);
	void RemoveInstanceMeshComponent(InstanceMeshComponent* instanMesh);
	void AddAttachMeshComponent(AttachMeshComponent* attachMeshComp);
	void RemoveAttachMeshComponent(AttachMeshComponent* attachMeshComp);
	void AddPointLight(PointLightComponent* light);
	void RemovePointLight(PointLightComponent* light);
	void ShowResource();                                                     // 登録されている テクスチャ・メッシュリソースの表示（デバッグ用）
	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } // ウィンドウ描画クリア
	void WindowFlip() { SDL_GL_SwapWindow(mWindow); }                        // ウィンドウフリップ
	void SetWindowTitle(const std::string& title);                           // ウィンドウタイトルのセット

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

	// Effekseer関連
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return mEffekseerRenderer; }
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager() { return mEffekseerManager; }

private:
	bool LoadShaders();                          // シェーダーの初期化
	void SetLightUniforms(class Shader* shader,const Matrix4& view); // ライト値をシェーダーにセット
	void CreateSpriteVerts();                    // スプライト頂点作成

	bool CreateMirrorTarget();
	void Draw3DScene(unsigned int frameBuffer, const Matrix4& view, const Matrix4& proj,float viewPortScale=1.0f,bool lit=true);
	void DrawFromGBuffer();

	int mScreenWidth;      // スクリーン幅                                                           
	int mScreenHeight;     // スクリーン高さ
	std::unordered_map<std::string, Texture*>   mTextures;			// テクスチャ登録配列
	std::unordered_map<std::string, Mesh*>      mMeshs;				// メッシュ登録配列
	std::vector<MeshComponent*>                 mMeshComponents;	// メッシュコンポーネント登録配列
	std::vector<InstanceMeshComponent*>			mInstanceMeshes;	// インスタンスメッシュコンポーネントの登録配列
	std::vector<SkeletalMeshComponent*>         mSkeletalMeshes;	// スケルタルメッシュの描画に使われる
	std::vector<AttachMeshComponent*>			mAttachMeshs;
	std::unordered_map<std::string, Skeleton*>  mSkeletons;			// スケルタルデータ
	std::unordered_map<std::string, Animation*> mAnims;				// アニメーションデータ
	std::unordered_map<const char16_t*, EffekseerEffect*> mEffects;


	//シェーダー関連
	Shader* mMeshShader;            // メッシュシェーダー
	Shader* mMeshShadowHDRShader;   // 通常メッシュシャドウシェーダー
	Shader* mSkinnedShader;         // スキンメッシュシェーダー
	Shader* mSkinnedDepthShader;    // スキンデプス用シェーダー 
	Shader* mSkinnedShadowHDRShader;// スキンシャドウシェーダー
	Shader* mSpriteShader;          // スプライトシェーダー
	Shader* mTilemapShader;         // タイルマップシェーダ
	Shader* mInstanceShader;		// インスタンス用シェーダー
	Shader* mUIShader;				// UI用シェーダー

	DepthMap* mDepthMapRender;		// デプスマップレンダラー
	HDRRenderer* mHDRRenderer;      // HDR レンダラー
	CubeMap* mCubeMap;				// キューブマップ
	PostEffect* mPostEffect;		// ポストエフェクト

	// 基本行列関連
	Matrix4                                           mView;             // ビュー行列
	Matrix4                                           mProjection;       // プロジェクション行列

	// スプライト頂点配列
	VertexArray* mSpriteVerts;

	// ライティング関連
	Vector3                                           mAmbientLight;     // アンビエントライト
	DirectionalLight                                  mDirectionalLight; // ディレクショナルライト

	// レンダリングベース情報関連
	SDL_Window* mWindow;           // SDLウィンドウハンドル 
	SDL_GLContext                                     mContext;          // OpenGLコンテキスト
	SDL_Renderer* mSDLRenderer;      // SDLレンダリングハンドル

	unsigned int mUndefineTexID;    // 未設定テクスチャの場合に割り当てられる黒色テクスチャ

	// Effekseer関連
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> mEffekseerRenderer; // Effekseerレンダラ
	Effekseer::RefPtr<Effekseer::Manager>            mEffekseerManager; // Effekseerマネージャ  
	
	InstanceMeshManager* mInstanceMeshManager;						// インスタンスマネージャー

	// ジオメトリインスタンス
	//std::unordered_map<InstanceType, GameObject*>;

	// Gバッファ関連
	GBuffer* mGBuffer;
	Shader* mGGlobalShader;
	Shader* mGPointLightShader;
	std::vector<PointLightComponent*> mPointLights;
	Mesh* mPointLightMesh;

	unsigned int mMirrorBuffer;
	Texture* mMirrorTexture;
	Matrix4 mMirrorView;
};

bool GLErrorHandle(const char* location);                              // OpenGLのエラーハンドル取得