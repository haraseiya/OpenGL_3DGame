// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Renderer.h"
#include "Collision.h"
#include "rapidjson/rapidjson.h"

class Renderer;
class Texture;
class VertexArray;

class Mesh
{
public:
	Mesh();
	~Mesh();

	// ���b�V���̃��[�h�E�A�����[�h
	bool Load(const std::string& fileName,Renderer* renderer); 
	void Unload();

	// ���b�V���̒��_�z��̎擾
	VertexArray* GetVertexArray() { return mVertexArray; }

	// �w�肳�ꂽ�C���f�b�N�X����e�N�X�`���̎擾
	Texture* GetTexture(int index);

	int GetTextureID(TextureStage stage);

	// �V�F�[�_�[���̎擾
	const std::string& GetShaderName() const { return mShaderName; }     

	// �R���W�����{�b�N�X���擾�i�I�u�W�F�N�g��ԁj
	const AABB& GetCollisionBox() const { return mBox; }

	// �o�E���f�B���O�X�t�B�A�̔��a���擾
	float GetRadius() const { return mRadius; }  

	// �X�y�L�������x���擾
	float GetSpecPower() const { return mSpecPower; }

	// ���~�i���X���x���擾
	float GetLuminace() const { return mLuminance; }

private:
	AABB mBox;							// AABB�R���W����
	std::vector<Texture*> mTextures;    // ���b�V���̃e�N�X�`��
	VertexArray* mVertexArray;          // ���b�V���̒��_�z��

	std::string mShaderName;	// ���b�V���w��̃V�F�[�_�[

	float mRadius;	            // �o�E���f�B���O�X�t�B�A�̔��a
	float mSpecPower;	        // �\�ʂ̃X�y�L�����[�l
	float mLuminance;           // ���Ȕ������x(HDR)

	std::unordered_map<TextureStage, int> mStageDefTexture;  // �e�N�X�`���X�e�[�W�Ɋ��蓖�Ă�ꂽ�e�N�X�`��

};