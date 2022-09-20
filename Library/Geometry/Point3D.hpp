
#ifndef POINT3D_HPP
#define POINT3D_HPP

#include <compare>

namespace st::geometry
{

	/*! \brief Point in 3d space
	 *         Brief description continued.
	 *
	 *  Detailed description starts here.
	 */
	class Point3D
	{
	public:
		Point3D() noexcept;
		Point3D(float x, float y, float z) noexcept;


		auto operator<=>(const Point3D&) const = default;

		constexpr float x() const noexcept;
		constexpr float y() const noexcept;
		constexpr float z() const noexcept;
		constexpr void setX(float x) noexcept;
		constexpr void setY(float y) noexcept;
		constexpr void setZ(float z) noexcept;



        constexpr inline Point3D& operator+=(const Point3D& p)
        {
            m_x += p.m_x;
            m_y += p.m_y;
            m_z += p.m_z;

            return *this;
        }

		constexpr inline Point3D& operator-=(const Point3D& p)
        {
            m_x -= p.m_x;
            m_y -= p.m_y;
            m_z -= p.m_z;

            return *this;
        }

	private:
		float m_x;
		float m_y;
		float m_z;

	};
    

}


#endif // !POINT3D_HPP