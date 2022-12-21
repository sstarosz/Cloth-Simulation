#ifndef VIEWPORT_CAMERA_HPP
#define VIEWPORT_CAMERA_HPP


#include <vector>
#include <numbers>
#include <vulkan/vulkan.hpp>
#include <Geometry/Matrix4x4.hpp>
#include <Geometry/Vector3.hpp>


namespace st::viewport
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

        
        void mousePressEvent(int64_t x, int64_t y, Actions action);

        void orbit(float dx, float dy);

        void setMousePosition(int64_t x, int64_t y);
        void releaseMouseClick();

        geometry::Matrix4x4 getViewMatrix() const;


    private:

        Actions m_currentState;

        geometry::Vector3 m_eye;
        geometry::Vector3 m_center;
        geometry::Vector3 m_up;

        float m_fov = 60.0f;


        float m_mouseClickX;
        float m_mouseClickY;

        
        uint64_t m_cameraHeight;
        uint64_t m_cameraWidth;

    };


};

#endif // SWAPCHAINSUPPORT_HPP
