#ifndef GEOMETRY_VECTOR3_HPP
#define GEOMETRY_VECTOR3_HPP

#include <compare>
#include <cassert>

namespace st::geometry
{

    /*! \brief Point in 3d space
         *         Brief description continued.
         *
         *  Detailed description starts here.
         */
    class Vector3
    {
    public:
        Vector3() noexcept;
        Vector3(float x, float y, float z) noexcept;


        constexpr float& operator[](const size_t index)
        {
            assert(index >= 0 && index < 4);
            return m_value[index];
        }
         
        const float& operator[](const size_t index) const
        {
            assert(index >= 0 && index < 4);
            return m_value[index];
        }

        auto operator<=>(const Vector3&) const = default;


        Vector3 operator+(const Vector3& v) const;
        Vector3 operator-(const Vector3& v) const;
        Vector3 operator*(const float& v) const;
        Vector3 operator*=(const float& v);


       

        static float lenght(const Vector3& vec);
        static Vector3 normalize(const Vector3& vec);
        static Vector3 crossProduct(const Vector3& v, const Vector3& w);

    private:
        float m_value[3];
    };




}


#endif // !GEOMETRY_OBJECT3D_HPP
