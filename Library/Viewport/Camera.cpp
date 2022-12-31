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
			break;
		case Camera::Actions::Pan:
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
		Vector3 origin = m_center;
		Vector3 position = m_eye;

		// Get the length of sight
		Vector3 centerToEye{ position - origin };
		float radius = Vector3::lenght(centerToEye);
		centerToEye = Vector3::normalize(centerToEye);


		// Find the rotation around the UP axis (Y)
		Vector3 axeZ(Vector3::normalize(centerToEye));
		//Problem hire
		Matrix4x4 rotY = Matrix4x4::rotationAroundAxis(-dx, m_up);

		// Apply the (Y) rotation to the eye-center vector
		Vector4 vect_temp = rotY * Vector4(centerToEye, 0.0F);
		centerToEye = Vector3(vect_temp[0], vect_temp[1], vect_temp[2]);


		// Find the rotation around the X vector: cross between eye-center and up (X)
		Vector3 axeX = Vector3::crossProduct(m_up, axeZ);
		axeX = Vector3::normalize(axeX);
		Matrix4x4 rotX = Matrix4x4::rotationAroundAxis(-dy, axeX);

		// Apply the (X) rotation to the eye-center vector
		vect_temp = rotX * Vector4(centerToEye, 0.0F);
		Vector3 vectRot{ vect_temp[0], vect_temp[1], vect_temp[2] };

		if (sign(vectRot[0]) == sign(centerToEye[0]))
		{
			centerToEye = vectRot;
		}

		centerToEye *= radius;

		m_eye = centerToEye + origin;
	}


	geometry::Vector3 Camera::orbitTest(float dx, float dy)
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
		Vector3 origin = m_center;
		Vector3 position = m_eye;

		// Get the length of sight
		Vector3 centerToEye{ position - origin };
		float radius = Vector3::lenght(centerToEye);
		centerToEye = Vector3::normalize(centerToEye);


		// Find the rotation around the UP axis (Y)
		Vector3 axeZ(Vector3::normalize(centerToEye));
		//Problem hire
		Matrix4x4 rotY = Matrix4x4::rotationAroundAxis(-dx, m_up);

		// Apply the (Y) rotation to the eye-center vector
		Vector4 vect_temp = rotY * Vector4(centerToEye, 0.0F);
		centerToEye = Vector3(vect_temp[0], vect_temp[1], vect_temp[2]);


		// Find the rotation around the X vector: cross between eye-center and up (X)
		Vector3 axeX = Vector3::crossProduct(m_up, axeZ);
		axeX = Vector3::normalize(axeX);
		Matrix4x4 rotX = Matrix4x4::rotationAroundAxis(-dy, axeX);

		// Apply the (X) rotation to the eye-center vector
		vect_temp = rotX * Vector4(centerToEye, 0.0F);
		Vector3 vectRot{ vect_temp[0], vect_temp[1], vect_temp[2] };

		if (sign(vectRot[0]) == sign(centerToEye[0]))
		{
			centerToEye = vectRot;
		}

		centerToEye *= radius;

		Vector3 newPosition = centerToEye + origin;
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

	geometry::Matrix4x4 Camera::lookAt(const geometry::Vector3& eye, const geometry::Vector3& center, const geometry::Vector3& up)
	{
		using namespace geometry;

		Matrix4x4 result;
		Vector3 x;
		Vector3 y;
		Vector3 z;

		//Z vector
		z[0] = eye[0] - center[0];
		z[1] = eye[1] - center[1];
		z[2] = eye[2] - center[2];
		z = Vector3::normalize(z);


		//Y vector
		y[0] = up[0];
		y[1] = up[1];
		y[2] = up[2];

		x = Vector3::crossProduct(y, z);

		y = Vector3::crossProduct(z, x);


		x = Vector3::normalize(x);
		y = Vector3::normalize(y);


		result[0] =  x[0];
		result[1] =  x[1];
		result[2] =  x[2];
		result[3] = -x[0] * eye[0] - x[1] * eye[1] - x[2] * eye[2];

		result[4] =  y[0];
		result[5] =  y[1];
		result[6] =  y[2];
		result[7] = -y[0] * eye[0] - y[1] * eye[1] - y[2] * eye[2];

		result[8]  =  z[0];
		result[9]  =  z[1];
		result[10] =  z[2];
		result[11] = -z[0] * eye[0] - z[1] * eye[1] - z[2] * eye[2];


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


