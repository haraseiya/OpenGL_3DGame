#pragma once
#include <vector>
#include <unordered_map>
#include <string>

class UIScreen;
class HUD;
class Font;

class UIManager
{
public:
	// インスタンスを取得
	static UIManager* GetInstance() { return mManager; }

	// UIマネージャーインスタンスを生成・破棄
	static void CreateInstance();	
	static void DeleteInstance();

	// UIスクリーンを更新
	void Update(float deltaTime);	

	// UIスクリーンを追加
	void AddUIScreen(UIScreen* screen);

	// 全てのUIスクリーンの参照を返す
	const std::vector<UIScreen*>& GetUIScreenStack() { return mUIScreenStack; }

	// フォントを取得
	Font* GetFont(const std::string& fileName);

	// テキストを取得
	const std::string& GetText(const std::string& key);

private:
	// コンストラクタ・デストラクタ
	UIManager();
	~UIManager() {}

	static UIManager* mManager;				// UIマネージャー

	std::vector<UIScreen*> mUIScreenStack;	// UIスクリーンを格納する配列
	
	std::unordered_map<std::string, Font*> mFonts;	// フォント
	std::unordered_map<std::string, std::string> mText;
};

#define UI_MANAGER UIManager::GetInstance();