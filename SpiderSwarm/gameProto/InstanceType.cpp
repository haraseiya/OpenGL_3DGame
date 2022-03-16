#include "InstanceType.h"
#include <stdexcept>

InstanceType& operator++(InstanceType& rhs)
{
	if (rhs == InstanceType::End)
	{
		throw std::out_of_range("++É~ÉX");
	}rhs = InstanceType(static_cast<std::underlying_type<InstanceType>::type>(rhs) + 1);
	return rhs;
}