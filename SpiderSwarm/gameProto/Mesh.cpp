#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "Math.h"
#include "RapidJsonHelper.h"

int LoadStageTextures(const rapidjson::Document& doc, TextureStage texStage, const char* stgString);

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};
}

Mesh::Mesh()
	: mVertexArray(nullptr)
	, mRadius(0.0f)
	, mSpecPower(100.0f)
	, mLuminance(1.0f)
{
	mStageDefTexture.emplace(TextureStage::DiffuseMap, 0);
	mStageDefTexture.emplace(TextureStage::NormalMap, 0);
	mStageDefTexture.emplace(TextureStage::SpecularMap, 0);
	mStageDefTexture.emplace(TextureStage::EmissiveMap, 0);
}

Mesh::~Mesh()
{
}

// メッシュのロード
bool Mesh::Load(const std::string& fileName, Renderer* renderer)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		printf("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	// JSONの解析を行う
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	// JSONオブジェクトか？
	if (!doc.IsObject())
	{
		printf("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// バージョンチェック
	if (ver != 1)
	{
		printf("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// 頂点レイアウトとサイズをファイルからセット
	VertexArray::Layout layout= VertexArray::Layout::PosNormTex;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::Layout::PosNormSkinTex;
		// This is the number of "Vertex" unions, which is 8 + 2 (for skinning)s　1個の頂点の集合の数　８　＋　２（スキニング分）
		// 3 (位置xyz) + 3(法線xyz) + 2(Boneと重み）＋　2(UV)  の計　10個分（40byte) 　
		vertSize = 10;
	}

	// テクスチャのロード
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		printf("Mesh %s has no textures, there should be at least one", fileName.c_str());
		return false;
	}

	// スペキュラー強度
	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	// エミッシブ強度値（定義されていれば)
	if (IsExistMember(doc, "luminance"))
	{
		mLuminance = ForceGetFloat(doc["luminance"]);
	}

	// テクスチャ読み込み（旧ファイル形式)
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		// このテクスチャ既に読みこんでる？
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr)
		{
			// テクスチャ読み込みのトライ
			t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// NULLのままであればデフォルトテクスチャをセット
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);

		if (textures.Size() == 1)
		{
			mStageDefTexture[TextureStage::DiffuseMap] = t->GetTextureID();
		}
	}

	// テクスチャ読み込み(新ファイル形式)
	if (IsExistMember(doc, "diffusemap"))
	{
		mStageDefTexture[TextureStage::DiffuseMap] = LoadStageTextures(doc, TextureStage::DiffuseMap, "diffusemap");
	}
	mStageDefTexture[TextureStage::NormalMap] = LoadStageTextures(doc, TextureStage::NormalMap, "normalmap");
	mStageDefTexture[TextureStage::SpecularMap] = LoadStageTextures(doc, TextureStage::SpecularMap, "specularmap");
	mStageDefTexture[TextureStage::EmissiveMap] = LoadStageTextures(doc, TextureStage::EmissiveMap, "emissivemap");

	// 頂点読み込み
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		printf("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	// 頂点データ
	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// 現時点で、８つの要素(位置xyz 法線xyz テクスチャuvの要素）が入っている
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			printf("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		// 頂点位置から　バウンディングスフィアの半径を計算する（原点からの距離が最大のものを半径とする）
		Vector3 pos(static_cast<float>(vert[0].GetDouble()),
			static_cast<float>(vert[1].GetDouble()),
			static_cast<float>(vert[2].GetDouble()));
		mRadius = Math::Max(mRadius, pos.LengthSq());
		// バウンディングボックスも計算
		if (i == 0)
		{
			mBox.InitMinMax(pos);
		}
		mBox.UpdateMinMax(pos);

		// 頂点レイアウトが PosNormTexなら（ボーンが無い）
		if (layout == VertexArray::Layout::PosNormTex)
		{
			Vertex v;
			// Add the floats　float値を追加
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		else if(layout==VertexArray::Layout::PosNormSkinTex)// ボーンデータ入りなら　PosNormSkinTexなら
		{
			Vertex v;
			// Add pos/normal　頂点と法線を追加　6個分
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// Add skin information　スキン情報追加（ボーン番号:unsigned charの1バイト分）
			for (rapidjson::SizeType j = 6; j < 14; j += 4)  //ループとしては2回転する　1回転目はボーン番号、2回転目はボーンウェイトをintとして取ってくる（使用時に浮動小数化するっぽい）
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// Add tex coords　テクスチャ座標
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
	}

	// バウンディングスフィアの半径を計算
	mRadius = Math::Sqrt(mRadius);

	// 頂点インデックスをロード
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		printf("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	//頂点インデックスを引っ張ってくる
	std::vector<unsigned int> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			printf("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// 頂点配列を作成する
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		layout, indices.data(), static_cast<unsigned>(indices.size()));
	return true;
}

void Mesh::Unload()
{
	delete mVertexArray;
	mVertexArray = nullptr;

}

Texture* Mesh::GetTexture(int index)
{
	if (index < static_cast<int>(mTextures.size()))
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}

int Mesh::GetTextureID(TextureStage stage)
{
	return mStageDefTexture[stage];
}

int LoadStageTextures(const rapidjson::Document& doc, TextureStage texStage, const char* stgString)
{
	std::string noneTexture("none");
	std::string texName;
	Texture* t;
	if (IsExistMember(doc, stgString))
	{
		texName = doc[stgString].GetString();
		if (texName != noneTexture)
		{
			t = RENDERER->GetTexture(texName);
			if (t == nullptr)
			{
				t = RENDERER->GetTexture("Assets/Default.png");
			}
			return t->GetTextureID();
		}
	}
	return 0;
}
