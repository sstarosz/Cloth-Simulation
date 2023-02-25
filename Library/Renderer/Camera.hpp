#ifndef RENDERER_CAMERA_HPP
#define RENDERER_CAMERA_HPP


#include <vector>
#include <numbers>
#include <vulkan/vulkan.hpp>
#include <Geometry/Matrix4x4.hpp>
#include <Geometry/Vector3.hpp>
#include <Geometry/Vector4.hpp>

namespace st::renderer
{

	class Camera
	{
	public:
		enum class Actions : uint64_t
		{
			NoAction,
			Orbit,
			Zoom,
			Pan
		};

		Camera();

		void mousePressEvent(int64_t x, int64_t y, Actions action);
		void mouseMove(int64_t x, int64_t y);
		void releaseMouseClick();


		void update();

		geometry::Matrix4x4 lookAt(const geometry::Vector3& eye, const geometry::Vector3& center, const geometry::Vector3& up);
		void orbit(float dx, float dy);
		void pan(float dx, float dy);
		void dolly(float dx, float dy);
		geometry::Vector3 orbitTest(float dx, float dy);


		geometry::Matrix4x4 getViewMatrix() const;
		geometry::Matrix4x4 getProjectionMatrix(float fovy, float aspect, float nearPlane, float farPlane) const;


	private:
		Actions m_currentState;

		geometry::Vector3 m_eye;
		geometry::Vector3 m_center;
		geometry::Vector3 m_up;

		geometry::Matrix4x4 m_matrix;

		float m_fov = 60.0f;


		float m_mouseClickX;
		float m_mouseClickY;


		uint64_t m_cameraHeight;
		uint64_t m_cameraWidth;
	};


};

#endif // SWAPCHAINSUPPORT_HPP
