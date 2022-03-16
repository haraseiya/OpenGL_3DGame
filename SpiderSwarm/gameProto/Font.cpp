#include "Font.h"
#include "UIManager.h"
#include "Texture.h"

Font::Font(UIManager* manager)
	: mUIManager(manager)
{
}

Font::~Font()
{
}

// ������
void Font::Initialize()
{
	TTF_Init();
}

// �I������
void Font::Finalize()
{
	TTF_Quit();
}

bool Font::Load(const std::string& fileName)
{
	// �T�|�[�g����t�H���g�T�C�Y
	std::vector<int> fontSizes =
	{
		8,9,
		10,11,12,14,16,18,
		20,22,24,26,28,
		30,32,34,36,38,
		40,42,44,46,48,
		52,56,
		60,64,68,
		72
	};

	// �T�C�Y���Ƃ�TTF_OpenFont��1�񂸂Ăяo��
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("�t�H���g%s�T�C�Y%d�̃��[�h�̎��s���܂���",fileName.c_str(), size);
			return false;
		}
		mFontData.emplace(size, font);
	}
	return true;
}

void Font::UnLoad()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::RenderText(const std::string& text, const Vector3& color, int pointSize)
{
	Texture* texture = nullptr;

	// �F��SDL_Color�ɓo�^����
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// �w��T�C�Y�̃t�H���g�f�[�^��T��
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = mUIManager->GetText(text);

		// SDL_Surface�ɕ`��i�A���t�@�u�����f�B���O����j
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(),sdlColor);

		if (surf != nullptr)
		{
			// SDL_Surface����e�N�X�`���ɕϊ�����
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("�|�C���g�T�C�Y%d�����Ή��ł�", pointSize);
	}

	return texture;
}