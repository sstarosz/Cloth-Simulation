#include "Camera.hpp"


namespace st::viewport
{




	void Camera::mousePressEvent(int64_t x, int64_t y, Actions action)
	{
		m_currentState = action;
		m_mouseClickX = static_cast<float>(x);
		m_mouseClickY = static_cast<float>(y);
	}

	void Camera::orbit(float dx, float dy)
	{
		using namespace geometry;

		if (dx == 0 && dy == 0)
		{
			return;
		}

		// Full width will do a full turn
		dx *= std::numbers::pi_v<float>;
		dy *= std::numbers::pi_v<float>;

		// Get the camera
		Vector3 origin = m_center;
		Vector3 position = m_eye;

		// Get the length of sight
		Vector3 centerToEye{ position - origin };
		float radius = Vector3::lenght(centerToEye);
		centerToEye = Vector3::normalize(centerToEye);


		Matrix4x4 rotX;
		Matrix4x4 rotY;


		Vector3 axeZ(Vector3::normalize(centerToEye));
		//rotY = Matrix4x4::
	}

	void Camera::setMousePosition(int64_t x, int64_t y)
	{

	}

	geometry::Matrix4x4 Camera::getViewMatrix() const
	{
		return geometry::Matrix4x4();
	}

	geometry::Matrix4x4 Camera::getProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) const
	{
		geometry::Matrix4x4 result;

		float      f = farPlane;
		float      n = nearPlane;

		const float t = n * std::tan(fovy * 0.5F);
		const float b = -t;
		const float l = b * aspect;
		const float r = t * aspect;


		//Frist row
		result[0] = (2 * n) / (r - l);
		result[1] = 0.0F;
		result[2] = (r + l) / (r - l);
		result[3] = 0.0F;

		//Second row
		result[4] = 0.0F;
		result[5] = -(2 * n) / (t - b);
		result[6] = (t + b) / (t - b);
		result[7] = 0.0F;

		//Third row
		result[8] = 0.0F;
		result[9] = 0.0F;
		result[10] = -(f) / (f - n);
		result[11] = (f * n) / (n - f);

		//Fourth row
		result[12] =  0.0F;
		result[13] =  0.0F;
		result[14] = -1.0F;
		result[15] =  0.0F;

		return result;
	}
}



