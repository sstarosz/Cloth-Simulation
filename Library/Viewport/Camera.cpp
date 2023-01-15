#include "Camera.hpp"


namespace st::viewport
{
	Camera::Camera():
		m_currentState(Camera::Actions::NoAction),
		m_eye(0.0F,0.0F,2.0F),
		m_center(0.0F,0.0F,0.0F),
		m_up(0.0F, 1.0F, 0.0F),
		m_matrix(),
		m_mouseClickX(0.0F),
		m_mouseClickY(0.0F),
		m_cameraHeight(500),
		m_cameraWidth(800)
	{
		update();
	}


	inline float sign(float s)
	{
		return (s < 0.f) ? -1.f : 1.f;
	}


	void Camera::mousePressEvent(int64_t x, int64_t y, Actions action)
	{
		m_currentState = action;
		m_mouseClickX = static_cast<float>(x);
		m_mouseClickY = static_cast<float>(y);
	}

	void Camera::mouseMove(int64_t x, int64_t y)
	{
		float localX = static_cast<float>(x - m_mouseClickX) / static_cast<float>(m_cameraWidth);
		float localY = static_cast<float>(y - m_mouseClickY) / static_cast<float>(m_cameraHeight);

		switch (m_currentState)
		{
		case Camera::Actions::NoAction:
			return;
			break;
		case Camera::Actions::Orbit:
			orbit(localX, localY);
			break;
		case Camera::Actions::Zoom:
			dolly(localX, localY);
			break;
		case Camera::Actions::Pan:
			pan(localX, localY);
			break;
		default:
			break;
		}

		update();

		m_mouseClickX = static_cast<float>(x);
		m_mouseClickY = static_cast<float>(y);
	}

	void Camera::releaseMouseClick()
	{
		m_currentState = Camera::Actions::NoAction;
	}

	void Camera::orbit(float dx, float dy)
	{
		using namespace geometry;

		if (dx == 0 && dy == 0)
		{
			return;
		}

		// Full width will do a full turn
		dx *= 2 * std::numbers::pi_v<float>;
		dy *= 2 * std::numbers::pi_v<float>;

		// Get the camera
		Vector3D origin = m_center;
		Vector3D position = m_eye;

		// Get the length of sight
		Vector3D centerToEye{ position - origin };
		float radius = Vector3D::lenght(centerToEye);
		centerToEye = Vector3D::normalize(centerToEye);


		// Find the rotation around the UP axis (Y)
		Vector3D axeZ(Vector3D::normalize(centerToEye));
		//Problem hire
		Matrix4x4 rotY = Matrix4x4::rotationAroundAxis(-dx, m_up);

		// Apply the (Y) rotation to the eye-center vector
		Vector4 vect_temp = rotY * Vector4(centerToEye, 0.0F);
		centerToEye = Vector3D(vect_temp[0], vect_temp[1], vect_temp[2]);


		// Find the rotation around the X vector: cross between eye-center and up (X)
		Vector3D axeX = Vector3D::crossProduct(m_up, axeZ);
		axeX = Vector3D::normalize(axeX);
		Matrix4x4 rotX = Matrix4x4::rotationAroundAxis(-dy, axeX);

		// Apply the (X) rotation to the eye-center vector
		vect_temp = rotX * Vector4(centerToEye, 0.0F);
		Vector3D vectRot{ vect_temp[0], vect_temp[1], vect_temp[2] };

		if (sign(vectRot.x) == sign(centerToEye.x))
		{
			centerToEye = vectRot;
		}

		centerToEye *= radius;

		m_eye = centerToEye + origin;
	}


	void Camera::dolly(float dx, float dy)
	{
		using namespace geometry;
		Vector3D z = m_center - m_eye;
		const float lenght =Vector3D::lenght(z);

		if (lenght < 0.000001F)
		{
			return;
		}

		const float dd = std::fabs(dx) > std::fabs(dx) ? dx : -dy;

		float factor = 3.0F * dd;

		// Adjust speed based on distance.
		if (factor >= 1.0F)
		{
			return;
		}
		z *= factor;

		m_eye = m_eye + z;

	}


	void Camera::pan(float dx, float dy)
	{
		using namespace geometry;

		Vector3D z(m_eye - m_center);
		float         length = static_cast<float>(Vector3D::lenght(z)) / 0.785f;  // 45 degrees
		z = Vector3D::normalize(z);
		Vector3D x = Vector3D::crossProduct(m_up, z);
		x = Vector3D::normalize(x);
		Vector3D y = Vector3D::crossProduct(z, x);
		y = Vector3D::normalize(y);
		x *= -dx * length;
		y *= dy * length;

		m_eye = m_eye + x + y;
		m_center = m_center + x + y;
	}



	geometry::Vector3D Camera::orbitTest(float dx, float dy)
	{
		using namespace geometry;

		if (dx == 0 && dy == 0)
		{
			return {};
		}

		// Full width will do a full turn
		dx *= 2 * std::numbers::pi_v<float>;
		dy *= 2 * std::numbers::pi_v<float>;

		// Get the camera
		Vector3D origin = m_center;
		Vector3D position = m_eye;

		// Get the length of sight
		Vector3D centerToEye{ position - origin };
		float radius = Vector3D::lenght(centerToEye);
		centerToEye = Vector3D::normalize(centerToEye);


		// Find the rotation around the UP axis (Y)
		Vector3D axeZ(Vector3D::normalize(centerToEye));
		//Problem hire
		Matrix4x4 rotY = Matrix4x4::rotationAroundAxis(-dx, m_up);

		// Apply the (Y) rotation to the eye-center vector
		Vector4 vect_temp = rotY * Vector4(centerToEye, 0.0F);
		centerToEye = Vector3D(vect_temp[0], vect_temp[1], vect_temp[2]);


		// Find the rotation around the X vector: cross between eye-center and up (X)
		Vector3D axeX = Vector3D::crossProduct(m_up, axeZ);
		axeX = Vector3D::normalize(axeX);
		Matrix4x4 rotX = Matrix4x4::rotationAroundAxis(-dy, axeX);

		// Apply the (X) rotation to the eye-center vector
		vect_temp = rotX * Vector4(centerToEye, 0.0F);
		Vector3D vectRot{ vect_temp[0], vect_temp[1], vect_temp[2] };

		if (sign(vectRot[0]) == sign(centerToEye[0]))
		{
			centerToEye = vectRot;
		}

		centerToEye *= radius;

		Vector3D newPosition = centerToEye + origin;
		m_eye = newPosition;
		return newPosition;
	}

	void Camera::update()
	{
		m_matrix = lookAt(m_eye, m_center, m_up);
	}

	geometry::Matrix4x4 Camera::getViewMatrix() const
	{
		return m_matrix;
	}

	geometry::Matrix4x4 Camera::lookAt(const geometry::Vector3D& eye, const geometry::Vector3D& center, const geometry::Vector3D& up)
	{
		using namespace geometry;

		Matrix4x4 result;
		Vector3D x;
		Vector3D y;
		Vector3D z;

		//Z vector
		//TODO substract vectors
		z.x = eye.x - center.x;
		z.y = eye.y - center.y;
		z.z = eye.z - center.z;
		z = Vector3D::normalize(z);


		//Y vector
		y.x = up.x;
		y.y = up.y;
		y.z = up.z;

		x = Vector3D::crossProduct(y, z);

		y = Vector3D::crossProduct(z, x);


		x = Vector3D::normalize(x);
		y = Vector3D::normalize(y);


		result[0] =  x.x;
		result[1] =  x.y;
		result[2] =  x.z;
		result[3] = -x.x * eye.x - x.y * eye.y - x.z * eye.z;

		result[4] =  y.x;
		result[5] =  y.y;
		result[6] =  y.z;
		result[7] = -y.x * eye.x - y.y * eye.y - y.z * eye.z;

		result[8]  =  z.x;
		result[9]  =  z.y;
		result[10] =  z.z;
		result[11] = -z.x * eye.x - z.y * eye.y - z.z * eye.z;


		result[12] = 0.0F;
		result[13] = 0.0F;
		result[14] = 0.0F;
		result[15] = 1.0F;

		return result;
	}

	geometry::Matrix4x4 Camera::getProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) const
	{
		geometry::Matrix4x4 result;

		float      f = farPlane;
		float      n = nearPlane;

		const float t = n * std::tan(fovy * (std::numbers::pi_v<float> / 180.0F) * 0.5F);
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



