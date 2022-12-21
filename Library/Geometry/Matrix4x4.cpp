#include "Matrix4x4.hpp"
#include <cmath>

namespace st::geometry
{
	Matrix4x4::Matrix4x4() noexcept : m_value() { }

	Matrix4x4::Matrix4x4(float xa, float xb, float xc, float xd,
						 float ya, float yb, float yc, float yd,
						 float za, float zb, float zc, float zd,
						 float wa, float wb, float wc, float wd) noexcept
	{
		m_value[0]  = xa;
		m_value[1]  = xb;
		m_value[2]  = xc;
		m_value[3]  = xd;
		m_value[4]  = ya;
		m_value[5]  = yb;
		m_value[6]  = yc;
		m_value[7]  = yd;
		m_value[8]  = za;
		m_value[9]  = zb;
		m_value[10] = zc;
		m_value[11] = zd;
		m_value[12] = wa;
		m_value[13] = wb;
		m_value[14] = wc;
		m_value[15] = wd;
	}

	Matrix4x4 Matrix4x4::indentityMatrix()
	{
		return Matrix4x4(1.0, 0.0, 0.0, 0.0,
						 0.0, 1.0, 0.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 0.0, 0.0, 0.0, 1.0);
	}

	void Matrix4x4::translate(const Vector3 vector)
	{
		m_value[3]  += vector[0];
		m_value[7]  += vector[1];
		m_value[11] += vector[2];
	}

	Matrix4x4 Matrix4x4::projectionMatrix(float fieldOfView, float framebufferAspectRatio, float nearPlane, float farPlane)
	{
		//User se word in x right y up, z into screen
		Matrix4x4 result = Matrix4x4::indentityMatrix();

		result[5] = -1;
		result[10] = -1;

		Matrix4x4 projection{};
		projection[0] = std::pow(fieldOfView, -1) / std::tan(fieldOfView / 2.0F);
		projection[5] = 1 * std::tan(fieldOfView / 2.0F);
		projection[10] = farPlane / (farPlane - nearPlane);
		projection[11] = -nearPlane * (farPlane - nearPlane);
		projection[14] = 1;


		projection *= result;

		return projection;
	}

	


}
