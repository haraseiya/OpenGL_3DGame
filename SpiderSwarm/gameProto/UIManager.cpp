#include "UIManager.h"
#include "UIScreen.h"
#include "Font.h"

// UI�}�l�[�W���[�̃V���O���g����������
UIManager* UIManager::mManager = nullptr;

// UI�}�l�[�W���[�C���X�^���X�̐���
void UIManager::CreateInstance()
{
	// UI�}�l�[�W���[�����݂��Ȃ������琶��
	if (!mManager)
	{
		mManager = new UIManager();
	}
}

// UI�}�l�[�W���[�C���X�^���X�̔j��
void UIManager::DeleteInstance()
{
	// �}�l�[�W���[�����݂�����폜
	if (mManager)
	{
		delete mManager;
		mManager = nullptr;
	}
}

// UI�̍X�V����
void UIManager::Update(float deltaTime)
{
	// �A�N�e�B�u��Ԃ�UI��S�čX�V
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::ACTIVE)
		{
			ui->Update(deltaTime);
		}
	}

	// ���Ă����Ԃ�UI��S�Ĕj��
	for (auto ui : mUIScreenStack)
	{
		if (ui->GetState() == UIScreen::UIScreen::CLOSING)
		{
			delete ui;
		}
	}
}

// UI�X�N���[����z��ɓo�^
void UIManager::AddUIScreen(UIScreen* screen)
{
	mUIScreenStack.emplace_back(screen);
}

// �t�H���g���擾
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

// �e�L�X�g�̎擾
const std::string& UIManager::GetText(const std::string& key)
{
	static std::string errorMsg(" �L�[��������܂���\n");

	// �e�L�X�g�����݂���ꍇ
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

// �R���X�g���N�^
UIManager::UIManager()
{

}
