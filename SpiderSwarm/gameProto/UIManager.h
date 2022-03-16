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
	// �C���X�^���X���擾
	static UIManager* GetInstance() { return mManager; }

	// UI�}�l�[�W���[�C���X�^���X�𐶐��E�j��
	static void CreateInstance();	
	static void DeleteInstance();

	// UI�X�N���[�����X�V
	void Update(float deltaTime);	

	// UI�X�N���[����ǉ�
	void AddUIScreen(UIScreen* screen);

	// �S�Ă�UI�X�N���[���̎Q�Ƃ�Ԃ�
	const std::vector<UIScreen*>& GetUIScreenStack() { return mUIScreenStack; }

	// �t�H���g���擾
	Font* GetFont(const std::string& fileName);

	// �e�L�X�g���擾
	const std::string& GetText(const std::string& key);

private:
	// �R���X�g���N�^�E�f�X�g���N�^
	UIManager();
	~UIManager() {}

	static UIManager* mManager;				// UI�}�l�[�W���[

	std::vector<UIScreen*> mUIScreenStack;	// UI�X�N���[�����i�[����z��
	
	std::unordered_map<std::string, Font*> mFonts;	// �t�H���g
	std::unordered_map<std::string, std::string> mText;
};

#define UI_MANAGER UIManager::GetInstance();