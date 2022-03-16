#include "Renderer.h"
#include "Game.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "DepthMap.h"
#include "HDRRenderer.h"
#include "CubeMap.h"
#include "PostEffect.h"
#include "EffekseerEffect.h"
#include "InstanceMeshManager.h"
#include "InstanceMeshComponent.h"
#include "GBuffer.h"
#include "PointLightComponent.h"
#include "UIManager.h"

Renderer::Renderer()
	: mWindow(nullptr)
	, mSDLRenderer(nullptr)
	, mContext(0)
	, mMeshShader(nullptr)
	, mSkinnedShader(nullptr)
	, mDepthMapRender(nullptr)
	, mHDRRenderer(nullptr)
	, mCubeMap(nullptr)
	, mPostEffect(nullptr)
	, mInstanceMeshManager(nullptr)
	, mGBuffer(nullptr)
	, mGGlobalShader(nullptr)
	, mGPointLightShader(nullptr)
	, mUndefineTexID(0)
	, mMirrorBuffer(0)
	, mMirrorTexture(nullptr)
{
}

Renderer::~Renderer()
{
	delete mDepthMapRender;
	delete mHDRRenderer;
	delete mCubeMap;
	delete mGBuffer;
}

bool Renderer::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// OpenGLのバージョンを4.3に初期化
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファリング
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを強制
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Windowの作成
	mWindow = SDL_CreateWindow("SDL & GL Window",+
		0, 0,
		mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!mWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	if (fullScreen)
	{
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) サイズのフルスクリーン化に失敗\n", screenWidth, screenHeight);
			return false;
		}
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}

	//SDLRendererの作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNGの初期化に失敗 : %s", SDL_GetError());
		return false;
	}

	// OpenGLContextの作成
	mContext = SDL_GL_CreateContext(mWindow);

	// Glewの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEWの初期化に失敗");
		return false;
	}
	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();

	// シェーダープログラムの初期化
	if (!LoadShaders())
	{
		printf("シェーダーの初期化に失敗");
		return false;
	}

	// 未定義テクスチャ
	mUndefineTexID = GetTexture("Assets/noneTexture.png")->GetTextureID();

	// デプスレンダラー初期化
	mDepthMapRender = new DepthMap;
	const int depthmapSize = 2048;
	mDepthMapRender->CreateShadowMap(depthmapSize);

	// HDRレンダラー初期化
	mHDRRenderer = new HDRRenderer(mScreenWidth, mScreenHeight, 4);

	// キューブマップ追加
	mCubeMap = new CubeMap();

	// ポストエフェクト追加
	mPostEffect = new PostEffect();

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// 2D テクスチャ用頂点初期化
	CreateSpriteVerts();

	// ミラー用のレンダリングターゲットを作成
	if (!CreateMirrorTarget())
	{
		SDL_Log("ミラー用のレンダリングターゲット作成に失敗しました。\n");
		return false;
	}
	mPointLightMesh = GetMesh("assets/Mesh/Bullet.gpmesh");

	// Effekseer初期化
	mEffekseerRenderer = ::EffekseerRendererGL::Renderer::Create(8000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
	mEffekseerManager = ::Effekseer::Manager::Create(8000);

	// Effecseer描画モジュール作成
	mEffekseerManager->SetSpriteRenderer(mEffekseerRenderer->CreateSpriteRenderer());
	mEffekseerManager->SetRibbonRenderer(mEffekseerRenderer->CreateRibbonRenderer());
	mEffekseerManager->SetRingRenderer(mEffekseerRenderer->CreateRingRenderer());
	mEffekseerManager->SetTrackRenderer(mEffekseerRenderer->CreateTrackRenderer());
	mEffekseerManager->SetModelRenderer(mEffekseerRenderer->CreateModelRenderer());

	// Effekseer用のテクスチャ・モデル・マテリアルローダー
	mEffekseerManager->SetTextureLoader(mEffekseerRenderer->CreateTextureLoader());
	mEffekseerManager->SetModelLoader(mEffekseerRenderer->CreateModelLoader());
	mEffekseerManager->SetMaterialLoader(mEffekseerRenderer->CreateMaterialLoader());

	// インスタンシング描画
	mInstanceMeshManager = new InstanceMeshManager();

	// GBuffer作成
	mGBuffer = new GBuffer();
	if (!mGBuffer->Create(mScreenWidth, mScreenHeight))
	{
		SDL_Log("Gバッファの作成に失敗しました\n");
		return false;
	}

	return true;
}

void Renderer::SetDepthSetting(const Vector3& centerWorldPos, const Vector3& lightDir, const Vector3& upVec, float lightDistance)
{
	mDepthMapRender->CalcLightSpaceMatrix(centerWorldPos, lightDir, upVec, lightDistance);
}

void Renderer::Shutdown()
{
	//// レンダリングターゲットのテクスチャがある場合、取り除く
	//if (mMirrorTexture != nullptr)
	//{
	//	glDeleteFramebuffers(1, &mMirrorBuffer);
	//	mMirrorTexture->Unload();
	//	delete mMirrorTexture;
	//}
	//テクスチャ破棄
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// メッシュ破棄
	for (auto i : mMeshs)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshs.clear();

	//シェーダー破棄
	mMeshShader->Unload();

	// スケルトンの破棄
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// アニメーションの破棄
	for (auto a : mAnims)
	{
		delete a.second;
	}

	// エフェクトの破棄
	for (auto e : mEffects)
	{
		delete e.second;
	}

	// Effekseer関連の破棄
	mEffekseerManager.Reset();
	mEffekseerRenderer.Reset();

	// GBufferの破棄
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}

	// ポイントライトを全て破棄
	while (!mPointLights.empty())
	{
		delete mPointLights.back();
	}

	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;

	// SDL系の破棄
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
	//// 最初にミラーテクスチャに描画
	//Draw3DScene(mMirrorBuffer, mMirrorView, mProjection, 0.25f);

	//// 3DシーンをGバッファに描画
	//Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);

	//// フレームバッファをゼロに戻す（画面のフレームバッファ）
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// GBufferからの描画
	//DrawFromGBuffer();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	///////////////////////////////////////////////////////////////////////////////
	// デプスレンダリングパス
	///////////////////////////////////////////////////////////////////////////////
	Matrix4 lightSpaceMat = mDepthMapRender->GetLightSpaceMatrix();
	mDepthMapRender->DepthRenderingBegin();
	{
		// スタティックメッシュを描画
		for (auto mc : mMeshComponents)
		{
			// 可視設定であれば
			if (mc->GetVisible())
			{
				mc->Draw(mDepthMapRender->GetDepthMapShader());
			}
		}

		// アタッチメッシュ描画
		for (auto ac : mAttachMeshs)
		{
			if (ac->GetVisible())
			{
				ac->Draw(mDepthMapRender->GetDepthMapShader());
			}
		}

		// スキンメッシュを描画
		mSkinnedDepthShader->SetActive();
		mSkinnedDepthShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		//mSkinnedDepthShader->SetVectorUniform("uHitColor", Color::Red);

		// スキンシェーダにパラメータセット
		for (auto sk : mSkeletalMeshes)
		{
			// 可視設定であれば
			if (sk->GetVisible())
			{
				sk->Draw(mSkinnedDepthShader);
			}
		}
	}
	mDepthMapRender->DepthRenderignEnd(); // デプスレンダリングパス終了

	//////////////////////////////////////////////////////////////////
	// シャドウ + HDR
	//////////////////////////////////////////////////////////////////

	//mPostEffect->PreUpdate();
	// HDR初期処理
	mHDRRenderer->HdrRecordBegin();
	{
		//シャドウ付きでメッシュを描画
		mMeshShadowHDRShader->SetActive();
		mMeshShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mMeshShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mMeshShadowHDRShader->SetIntUniform("uTexture", 0);

		// デプスマップをセット
		mMeshShadowHDRShader->SetIntUniform("depthMap", 4);// ライティング変数をセット
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, mDepthMapRender->GetDepthTexID());

		SetLightUniforms(mMeshShadowHDRShader,mView);

		// 全てのメッシュコンポーネントを描画
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(mMeshShadowHDRShader);
			}
		}

		// スキン＆シャドウメッシュ
		mSkinnedShadowHDRShader->SetActive();

		// ビュープロジェクション＆ライト空間行列セット
		mSkinnedShadowHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		mSkinnedShadowHDRShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMat);
		mSkinnedShadowHDRShader->SetIntUniform("uTexture", 0);
		mSkinnedShadowHDRShader->SetIntUniform("depthMap", 4);

		// ライトパラメータのセット
		SetLightUniforms(mSkinnedShadowHDRShader,mView);

		for (auto sk : mSkeletalMeshes)
		{
			if (sk->GetVisible())
			{
				sk->Draw(mSkinnedShadowHDRShader);
			}
		}

		// アタッチメッシュ描画
		for (auto ac : mAttachMeshs)
		{
			if (ac->GetVisible())
			{
				ac->Draw(mSkinnedShadowHDRShader);
			}
		}

		// UIスクリーンの配列をすべて取得
		for (auto ui : UIManager::GetInstance()->GetUIScreenStack())
		{
		}

		// αブレンドを有効化
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mInstanceMeshManager->Draw();

	}
	mHDRRenderer->HdrRecordEnd();

	//////////////////////////////////////////////////////////////////
	// HDR Bloom の作成＆合成
	/////////////////////////////////////////////////////////////////
	mHDRRenderer->HiBrightBlurCreate();
	mHDRRenderer->HdrTonemapAndBrightBlurCombine();

	// HDRBufferにレンダリングしたときのDepth情報をスクリーンにコピー
	mHDRRenderer->CopyDepthToScreen();

	// キューブマップの描画
	//mCubeMap->Draw();
	//mPostEffect->DifUpdate();
	// 当たり判定デバッグBoxの表示
	GAMEINSTANCE.GetPhysics()->DebugShowBox();

}

void Renderer::SetViewMatrix(const Matrix4& view)
{
	Matrix4 tmp = view;
	mView = view;

	// Effekseer に座標系を合わせて行列をセットする
	Effekseer::Matrix44 efCam;
	tmp.mat[0][0] *= -1;
	tmp.mat[0][1] *= -1;
	tmp.mat[1][2] *= -1;
	tmp.mat[2][2] *= -1;
	tmp.mat[3][2] *= -1;

	efCam = tmp;
	RENDERER->GetEffekseerRenderer()->SetCameraMatrix(efCam);
}

void Renderer::SetProjMatrix(const Matrix4& proj)
{
	mProjection = proj;

	// Effekseer に座標系を合わせて行列をセットする
	Matrix4 tmp = proj;
	tmp.mat[2][2] *= -1;
	tmp.mat[2][3] *= -1;

	Effekseer::Matrix44 eProj;
	eProj = tmp;
	RENDERER->GetEffekseerRenderer()->SetProjectionMatrix(eProj);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf("Load Texture Success : %s \n", fileName.c_str());
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshs.find(fileName);

	if (iter != mMeshs.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh;
		if (m->Load(fileName, this))
		{
			mMeshs.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	printf("Load Mesh Success : %s\n", fileName.c_str());
	return m;
}

void Renderer::AddMeshComponent(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComponents.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComponents.begin(), mMeshComponents.end(), mesh);
		mMeshComponents.erase(iter);
	}

}
void Renderer::AddInstanceMeshComponent(InstanceMeshComponent* instanceMesh)
{
	InstanceType type = instanceMesh->GetType();
	mInstanceMeshManager->Entry(instanceMesh,type);
}

void Renderer::RemoveInstanceMeshComponent(InstanceMeshComponent* instanceMesh)
{
	InstanceType type = instanceMesh->GetType();
	mInstanceMeshManager->Remove(instanceMesh, type);
}

void Renderer::AddAttachMeshComponent(AttachMeshComponent* attachMeshComp)
{
	if (attachMeshComp->GetIsSkeletal())
	{
		AttachMeshComponent* sk = static_cast<AttachMeshComponent*>(attachMeshComp);
		mAttachMeshs.emplace_back(sk);
	}
	else
	{
		mAttachMeshs.emplace_back(attachMeshComp);
	}
}

void Renderer::RemoveAttachMeshComponent(AttachMeshComponent* attachMeshComp)
{
	if (attachMeshComp->GetIsSkeletal())
	{
		AttachMeshComponent* sk = static_cast<AttachMeshComponent*>(attachMeshComp);
		auto iter = std::find(mAttachMeshs.begin(), mAttachMeshs.end(), sk);
		mAttachMeshs.erase(iter);
	}
	else
	{
		auto iter = std::find(mAttachMeshs.begin(), mAttachMeshs.end(), attachMeshComp);
		mAttachMeshs.erase(iter);
	}
}

void Renderer::AddPointLight(PointLightComponent* light)
{
	mPointLights.emplace_back(light);
}

void Renderer::RemovePointLight(PointLightComponent* light)
{
	auto iter = std::find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(iter);
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		 0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		 0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::Layout::PosNormTex, indices, 6);
}

bool Renderer::CreateMirrorTarget()
{
	int width = mScreenWidth / 4;
	int height = mScreenHeight / 4;

	// ミラーテクスチャ用のフレームバッファ生成
	glGenFramebuffers(1, &mMirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);

	// レンダリングに使用するテクスチャの生成
	mMirrorTexture = new Texture();
	mMirrorTexture->CreateForRendering(width, height, GL_RGB);

	// このターゲットにデプスバッファを追加
	GLuint depthBuffer;
	glGenBuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// フレームバッファの出力ターゲットとしてミラーテクスチャを張り付け
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mMirrorTexture->GetTextureID(), 0);

	// このフレームバッファに描画するバッファのリストを設定
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	// 全てが動作したことを確認する
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// うまくいかなかった場合はフレームバッファとテクスチャを削除
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
		mMirrorTexture = nullptr;
		return false;
	}

	return true;
}

// 
void Renderer::Draw3DScene(unsigned int frameBuffer, const Matrix4& view, const Matrix4& proj, float viewPortScale, bool lit)
{
	// 現在のフレームバッファをセット
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// 大きさに基づくビューポートサイズの設定
	glViewport(0, 0,
		static_cast<int>(mScreenWidth * viewPortScale),
		static_cast<int>(mScreenHeight * viewPortScale));

	// カラーバッファとデプスバッファのクリア
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// デプスバッファ有効、αブレンド無効
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// メッシュシェーダー有効化
	mMeshShader->SetActive();

	// ビュー、プロジェクション行列の更新
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);

	// ライティングの更新
	if (lit)
	{
		SetLightUniforms(mMeshShader, view);
	}
	for (auto sk : mSkeletalMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mSkinnedShader);
		}
	}
}

void Renderer::DrawFromGBuffer()
{
	// グローバルライティングパスの深度テストを無効にする
	glDisable(GL_DEPTH_TEST);

	// グローバルGバッファシェーダーを有効化
	mGGlobalShader->SetActive();

	// スプライトの頂点をセット
	mSpriteVerts->SetActive();

	// Gバッファのテクスチャをサンプルに設定
	mGBuffer->SetTextureActive();

	// 照明ユニフォームの設定
	SetLightUniforms(mGGlobalShader, mView);

	// 三角形の描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Gバッファの深度バッファをデフォルトのフレームバッファにコピー
	glBindBuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	glBlitFramebuffer(0, 0, mScreenWidth, mScreenHeight, 0, 0, mScreenWidth, mScreenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// デプステストを有効化にするが、デプスバッファへの書き込みを無効化
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// ポイントライトシェーダーとメッシュを有効化
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();

	// ビュープロジェクション行列をユニフォームにセット
	mGPointLightShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// サンプリング用のGバッファ、テクスチャをセット
	mGBuffer->SetTextureActive();

	// ポイントライトの色は既存の色に加える
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// ポイントライトを描画
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mGPointLightShader, mPointLightMesh);
	}
}


void Renderer::ShowResource()
{
	printf("\n\n<------------------ textures ------------------>\n");
	for (auto i : mTextures)
	{
		printf("texfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  meshes  ------------------->\n");
	for (auto i : mMeshs)
	{
		printf("meshfile %s\n", i.first.c_str());
	}
}

void Renderer::SetWindowTitle(const std::string& title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

void Renderer::SpriteDrawBegin()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	mTilemapShader->SetActive();
	mSpriteVerts->SetActive();
}

void Renderer::SpriteDrawEnd()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

// テクスチャの描画
void Renderer::DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scale, float alpha)
{
	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth() / xDivNum) * scale,
		static_cast<float>(texture->GetHeight() / yDivNum) * scale,
		1.0f);
	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x - (mScreenWidth * 0.5f),
			(mScreenHeight * 0.5f) - offset.y, 0.0f));

	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	mTilemapShader->SetMatrixUniform("uWorldTransform", world);
	mTilemapShader->SetIntUniform("uTileIndex", index);
	mTilemapShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	mTilemapShader->SetFloatUniform("uAlpha", alpha);

	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexture(Texture* texture, const Vector2& offset, float scale, float alpha)
{
	DrawTexture(texture, 0, 1, 1, offset, scale, alpha);
}

bool Renderer::LoadShaders()
{
	// スプライトシェーダーのロード
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	// ビュープロジェクション行列のセット
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight));
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	mTilemapShader = new Shader();
	if (!mTilemapShader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		return false;
	}
	mTilemapShader->SetActive();
	// ビュープロジェクション行列のセット
	mTilemapShader->SetMatrixUniform("uViewProj", viewProj);

	// メッシュシェーダー
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(mScreenWidth),
		static_cast<float>(mScreenHeight),
		1.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// スキンメッシュシェーダー
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mSkinnedShader->SetActive();
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);


	// デプススキンメッシュシェーダー
	mSkinnedDepthShader = new Shader();
	if (!mSkinnedDepthShader->Load("Shaders/SkinnedDepth.vert", "Shaders/Depthmap.frag"))
	{
		return false;
	}

	// 通常メッシュシャドウシェーダー
	mMeshShadowHDRShader = new Shader();
	if (!mMeshShadowHDRShader->Load("Shaders/ShadowMesh.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	// スキンシャドウシェーダー
	mSkinnedShadowHDRShader = new Shader();
	if (!mSkinnedShadowHDRShader->Load("Shaders/SkinnedShadow.vert", "Shaders/ShadowHDRMesh.frag"))
	{
		return false;
	}

	// インスタンス用シェーダー
	mInstanceShader = new Shader();
	if (!mInstanceShader->Load("Shaders/InstanceMesh.vert","Shaders/InstanceMesh.frag"))
	{
		return false;
	}

	// Gバッファから描画用のシェーダーを作成
	mGGlobalShader = new Shader();
	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", "Shaders/GBufferGlobal.frag"))
	{
		return false;
	}

	// 各サンプラーにインデックスを関連付ける
	mGGlobalShader->SetActive();
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);

	// ビューの投影はスプライトのものだけ
	mGGlobalShader->SetMatrixUniform("uViewProj", viewProj);

	// ワールド座標は画面に合わせて拡大縮小し、y軸を反転
	Matrix4 gBufferWorld = Matrix4::CreateScale(static_cast<float>(mScreenWidth), static_cast<float>(-mScreenHeight), 1.0f);
	mGGlobalShader->SetMatrixUniform("uWorldTransform", gBufferWorld);

	// GBufferからポイントライト用のシェーダーを作成
	mGPointLightShader = new Shader();
	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag"))
	{
		return false;
	}

	// サンプラーをセット
	mGPointLightShader->SetActive();
	mGPointLightShader->SetIntUniform("uGDiffuse", 0);
	mGPointLightShader->SetIntUniform("uGNormal", 1); 
	mGPointLightShader->SetIntUniform("uGWorldPos", 2);
	mGPointLightShader->SetVector2Uniform("uScreenDimensions", Vector2(static_cast<float>(mScreenWidth), static_cast<float>( mScreenHeight)));

	return true;
}

void Renderer::SetLightUniforms(Shader* shader,const Matrix4& view)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	//アンビエントライト
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	//ディレクショナルライト
	shader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
}

//////////////////////////////////////////////////////////////
// スケルタル情報の取得
// in  : スケルタル情報 .gpskelファイル名
// out : Skeleton情報へのポインタ
// Desc: gpskelファイル名より、スケルタル情報を返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpskelファイル名をキーとするスケルタル情報のmapが作成される
//////////////////////////////////////////////////////////////
const Skeleton* Renderer::GetSkeleton(const char* fileName)
{
	std::string file(fileName);
	auto iter = mSkeletons.find(file);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			mSkeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//////////////////////////////////////////////////////////////
// アニメーション情報の取得
// in  : アニメーションデータが格納されている .gpanimファイル名
// out : アニメーション情報へのポインタ
// Desc: gpanimファイル名よりアニメーションデータを返す。ない場合はそのファイル名で
//       読み込みを行う。読み込みを行ってもファイルが存在しない場合 nullptrを返す
//       内部でgpanimファイル名をキーとするアニメーション情報のmapが作成される
//////////////////////////////////////////////////////////////
const Animation* Renderer::GetAnimation(const char* fileName, bool loop)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName, loop))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

EffekseerEffect* Renderer::GetEffect(const char16_t* fileName)
{
	// エフェクトファイルを一度読み込んだかを確認
	EffekseerEffect* effect = nullptr;
	auto iter = mEffects.find(fileName);

	// 一度読んでいるなら返す
	if (iter != mEffects.end())
	{
		return iter->second;
	}

	//読んでいなければ新規追加
	effect = new EffekseerEffect;
	if (effect->LoadEffect(fileName))
	{
		mEffects.emplace(fileName, effect);
	}
	else
	{
		delete effect;
		effect = nullptr;
	}
	return effect;
}

bool GLErrorHandle(const char* location)
{
	GLenum error_code = glGetError();
	if (error_code == GL_NO_ERROR)
	{
		return true;
	}
	do
	{
		const char* msg;
		switch (error_code)
		{
		case GL_INVALID_ENUM:                  msg = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 msg = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             msg = "INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: msg = "unknown Error";
		}
		printf("GLErrorLayer: ERROR%04x'%s' location: %s\n", error_code, msg, location);
		error_code = glGetError();
	} while (error_code != GL_NO_ERROR);

	return false;
}
