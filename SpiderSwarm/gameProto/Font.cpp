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

// 初期化
void Font::Initialize()
{
	TTF_Init();
}

// 終了処理
void Font::Finalize()
{
	TTF_Quit();
}

bool Font::Load(const std::string& fileName)
{
	// サポートするフォントサイズ
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

	// サイズごとにTTF_OpenFontを1回ずつ呼び出す
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("フォント%sサイズ%dのロードの失敗しました",fileName.c_str(), size);
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

	// 色をSDL_Colorに登録する
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// 指定サイズのフォントデータを探す
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = mUIManager->GetText(text);

		// SDL_Surfaceに描画（アルファブレンディングする）
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(),sdlColor);

		if (surf != nullptr)
		{
			// SDL_Surfaceからテクスチャに変換する
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("ポイントサイズ%dが未対応です", pointSize);
	}

	return texture;
}
