#include "Vector3.hpp"

namespace st::geometry
{
	Vector3::Vector3() noexcept
	{
		m_value[0] = 0;
		m_value[1] = 0;
		m_value[2] = 0;

	}

	Vector3::Vector3(float x, float y, float z) noexcept
	{
		m_value[0] = x;
		m_value[1] = y;
		m_value[2] = z;
	}
}


