#include "Vector4.hpp"
#include <cmath>

namespace st::geometry
{
	Vector4::Vector4() noexcept
	{
		m_value[0] = 0;
		m_value[1] = 0;
		m_value[2] = 0;
		m_value[3] = 0;
	}

	Vector4::Vector4(float x, float y, float z, float w) noexcept
	{
		m_value[0] = x;
		m_value[1] = y;
		m_value[2] = z;
		m_value[3] = w;
	}

	Vector4::Vector4(Vector3D v, float w) noexcept
	{
		m_value[0] = v[0];
		m_value[1] = v[1];
		m_value[2] = v[2];
		m_value[3] = w;
	}

	Vector4 Vector4::operator+(const Vector4& v) const
	{
		return Vector4{ m_value[0] + v.m_value[0],
						m_value[1] + v.m_value[1],
						m_value[2] + v.m_value[2],
						m_value[3] + v.m_value[3] };
	}

	Vector4 Vector4::operator-(const Vector4& v) const
	{
		return Vector4{ m_value[0] - v.m_value[0],
						m_value[1] - v.m_value[1],
						m_value[2] - v.m_value[2],
						m_value[3] - v.m_value[3] };
	}

	Vector4 Vector4::operator*(const float& v) const
	{
		Vector4 result;
		result[0] = m_value[0] * v;
		result[1] = m_value[1] * v;
		result[2] = m_value[2] * v;
		result[3] = m_value[3] * v;

		return result;
	}


}


