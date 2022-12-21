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
		if (dx == 0 && dy == 0)
		{
			return;
		}


		dx *= std::numbers::pi_v<float>;
		dy *= std::numbers::pi_v<float>;


		geometry::Vector3 origin = m_center;
		geometry::Vector3 position = m_eye;

		geometry::Vector3 centerToEye{ position - origin };


	}

	void Camera::setMousePosition(int64_t x, int64_t y)
	{

	}

	geometry::Matrix4x4 Camera::getViewMatrix() const
	{
		return geometry::Matrix4x4();
	}
}



