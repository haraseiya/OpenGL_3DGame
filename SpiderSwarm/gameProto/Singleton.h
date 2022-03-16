#pragma once

template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	// �O���ł̃C���X�^���X�쐬�͋֎~
	Singleton() {}
	virtual ~Singleton() {}

	virtual void Create();
	virtual void Destroy();

private:
	void operator=(const Singleton& obj){}	// ������Z�q�֎~
	Singleton(const Singleton& obj) {}		// �R�s�[�R���X�g���N�^�֎~
};
