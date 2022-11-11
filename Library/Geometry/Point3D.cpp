#include "Point3D.hpp"


namespace st::geometry
{
Point3D::Point3D() noexcept: m_x(0.0), m_y(0.0), m_z(0.0) { }

Point3D::Point3D(float x, float y, float z) noexcept: m_x(x), m_y(y), m_z(z) { }

constexpr float Point3D::x() const noexcept { return m_x; }

constexpr float Point3D::y() const noexcept { return m_y; }

constexpr float Point3D::z() const noexcept { return m_z; }

constexpr void Point3D::setX(const float x) noexcept { m_x = x; }

constexpr void Point3D::setY(const float y) noexcept { m_y = y; }

constexpr void Point3D::setZ(const float z) noexcept { m_z = z; }


}
