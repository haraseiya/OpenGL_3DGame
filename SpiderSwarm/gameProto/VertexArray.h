// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class VertexArray
{
public:
	// ���_���C�A�E�g��
	enum class Layout
	{
		PosNormTex = 0,			// �ʒu&�@��&�e�N�X�`��UV ���������t�H�[�}�b�g
		PosNormSkinTex,			// �ʒu&�@��& "�X�L���p�̉e���{�[�����d�ݏ��" & �e�N�X�`��UV 
		PosNormTexInstanced,	// �C���X�^���X�p
	};

	// ���_�z��R���X�g���N�^ 
	VertexArray(const void* verts, unsigned int numVerts, Layout layout, 
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	void SetActive();                                          // ���̒��_�z����A�N�e�B�u�ɂ��ĕ`��Ŏg�p����
	unsigned int GetNumIndices() const { return mNumIndices; } // �C���f�b�N�X�����擾����
	unsigned int GetNumVerts() const { return mNumVerts; }     // ���_�����擾����
	
	Layout GetLayout()const { return mLayout; }
	void SetLayout(Layout layout) { mLayout = layout; }

	unsigned int GetVAO() { return mVertexArray; }		// ���_�z��̎擾

private:
	unsigned int mNumVerts;		// ���_��
	unsigned int mNumIndices;   // �C���f�b�N�X�� �i�|���S���ʐ��~�R)
	unsigned int mVertexBuffer; // ���_�o�b�t�@ID�iOpenGL�ɓo�^���ɕt�^�����j
	unsigned int mIndexBuffer;  // �C���f�b�N�X�o�b�t�@ID (OpenGL�o�^���ɕt�^�j
	unsigned int mInstanceBuffer;	// �C���X�^���X�p�̃o�b�t�@ID
	unsigned int mVertexArray;  // ���_�z��I�u�W�F�N�gID
	unsigned int mAmount;

	Layout mLayout;
};