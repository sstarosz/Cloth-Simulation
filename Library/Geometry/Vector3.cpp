#include "Vector3.hpp"

#include <cmath>

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


	Vector3 Vector3::operator+(const Vector3& v) const
	{
		return Vector3{ m_value[0] + v.m_value[0],
						m_value[1] + v.m_value[1],
						m_value[2] + v.m_value[2] };
	}

	Vector3 Vector3::operator-(const Vector3& v) const
	{
		return Vector3{ m_value[0] - v.m_value[0],
						m_value[1] - v.m_value[1],
						m_value[2] - v.m_value[2] };
	}

	Vector3 Vector3::operator*(const float& v) const
	{
		Vector3 result;
		result[0] = m_value[0] * v;
		result[1] = m_value[1] * v;
		result[2] = m_value[2] * v;

		return result;
	}

	float Vector3::lenght(const Vector3& vec)
	{
		return std::sqrtf(vec.m_value[0] * vec.m_value[0] + 
						  vec.m_value[1] * vec.m_value[1] +
						  vec.m_value[2] * vec.m_value[2]);
	}
	Vector3 Vector3::normalize(const Vector3& vec)
	{
		Vector3 u{ vec };
		float norm = std::sqrtf(vec.m_value[0] * vec.m_value[0] +
								vec.m_value[1] * vec.m_value[1] +
								vec.m_value[2] * vec.m_value[2]);
		
		if (norm > 10e-6)
		{
			norm = 1.0F / norm;
		}
		else
		{
			norm = 0.0F;
		}

		return u * norm;
	}
}


