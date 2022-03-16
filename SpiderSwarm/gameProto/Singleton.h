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
	// 外部でのインスタンス作成は禁止
	Singleton() {}
	virtual ~Singleton() {}

	virtual void Create();
	virtual void Destroy();

private:
	void operator=(const Singleton& obj){}	// 代入演算子禁止
	Singleton(const Singleton& obj) {}		// コピーコンストラクタ禁止
};
