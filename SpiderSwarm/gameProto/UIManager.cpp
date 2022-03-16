#include "UIManager.h"
#include "UIScreen.h"
#include "Font.h"

// UIマネージャーのシングルトンを初期化
UIManager* UIManager::mManager = nullptr;

// UIマネージャーインスタンスの生成
void UIManager::CreateInstance()
{
	// UIマネージャーが存在しなかったら生成
	if (!mManager)
	{
		mManager = new UIManager();
	}
}

// UIマネージャーインスタンスの破棄
void UIManager::DeleteInstance()
{
	// マネージャーが存在したら削除
	if (mManager)
	{
		delete mManager;
		mManager = nullptr;
	}
}

// UIの更新処理
void UIManager::Update(float deltaTime)
{
	// アクティブ状態のUIを全て更新
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(deltaTime);
		}
	}

	// 閉じている状態のUIを全て破棄
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::UIScreen::CLOSING)
		{
			delete ui;
		}
	}
}

// UIスクリーンを配列に登録
void UIManager::AddUIScreen(UIScreen* screen)
{
	mUIScreenStack.emplace_back(screen);
}

// フォントを取得
Font* UIManager::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->UnLoad();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

// テキストの取得
const std::string& UIManager::GetText(const std::string& key)
{
	static std::string errorMsg(" キーが見つかりません\n");

	// テキストが存在する場合
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

// コンストラクタ
UIManager::UIManager()
{

}
