// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

/////////////////////////////////////////////////////////////////
// ���_���C�A�E�g
// PosNormTex = 8 * sizeof(float) = 32 bytes
// | �ʒu      | �@��      | Texture|
// | x | y | z | x | y | z | u | v | 
//
// posNormSkinTex
// PosNormSkinTex = (8 * sizeof(float)) + (8 * sizeof(char)) = 40 bytes
// | �ʒu      | �@��       | Bones   | weight  |Texture|
// | x | y | z | x | y | z | char[4] | char[4] | u | v |
//                                    ��weight�̊m�ۂ�char�����A���x���K�v�Ȃ��̂�8bit�Œ菬���Ƃ��Ďg�p����

#include "VertexArray.h"
#include "Math.h"
#include <GL/glew.h>

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
	, mAmount(1000)
{
	// ���_�z��̍쐬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���_���C�A�E�g�� �X�P���^�����f���Ȃ�@�{�[��ID�A�e���x�����T�C�Y���₷
	unsigned vertexSize = 8 * sizeof(float);
	if (layout == VertexArray::Layout::PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}

	// ���_���C�A�E�g���C���X�^���X�p�ł����
	if (layout == VertexArray::Layout::PosNormTexInstanced)
	{
		vertexSize = sizeof(Matrix4);
	}

	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// �{�[���Ȃ�
	if (layout == VertexArray::Layout::PosNormTex)
	{
		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	// �{�[������
	else if (layout == VertexArray::Layout::PosNormSkinTex)
	{
		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		//�@�e���󂯂�{�[���C���f�b�N�X�ԍ�  (int�^���L�[�v)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
		// �{�[���E�F�C�g��� (float �ɕϊ�)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	// �{�[���Ȃ��C���X�^���X�p�̏ꍇ
	else if (layout == VertexArray::Layout::PosNormTexInstanced)
	{
		// float 3���@���@�ʒu x,y,z�@�ʒu�������Z�b�g
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ����float 3�� �� �@�� nx, ny, nz�@�@���������Z�b�g
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));
		// ����float 2�� u, v  �e�N�X�`�����W�������Z�b�g
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// ���_�A�g���r���[�g�ɍs����Z�b�g
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(1 * sizeof(float) * 4));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(2 * sizeof(float) * 4));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(3 * sizeof(float) * 4));

		// �����P�F���_�z��̃C���f�b�N�X���w��
		// �����Q�F�X���b�g�ł̔ėp�����̍X�V���œn�����C���X�^���X�̐����w�肷��
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
