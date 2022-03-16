#include "Tag.h"

// �O�u�C���N�������g
Tag& operator ++(Tag& rhs)
{
	if (rhs == Tag::END)
	{
		throw std::out_of_range("for outof range Tag& operator ++ (Tag&)");
	}
	rhs = Tag(static_cast<std::underlying_type<Tag>::type>(rhs) + 1);

	return rhs;
}