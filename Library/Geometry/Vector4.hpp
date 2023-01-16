#ifndef GEOMETRY_VECTOR4_HPP
#define GEOMETRY_VECTOR4_HPP

#include <compare>
#include <cassert>
#include "Vector3.hpp"
#include "Matrix4x4.hpp"

namespace st::geometry
{

    /*! \brief Point in 3d space
         *         Brief description continued.
         *
         *  Detailed description starts here.
         */
    class Vector4
    {
    public:
        Vector4() noexcept;
        Vector4(float x, float y, float z, float w) noexcept;
        Vector4(Vector3D v, float w) noexcept;


        constexpr float& operator[](const size_t index)
        {
            assert(index >= 0 && index <= 4);
            return m_value[index];
        }

        const float& operator[](const size_t index) const
        {
            assert(index >= 0 && index <= 4);
            return m_value[index];
        }

        auto operator<=>(const Vector4&) const = default;


        Vector4 operator+(const Vector4& v) const;
        Vector4 operator-(const Vector4& v) const;
        Vector4 operator*(const float& v) const;


    private:
        float m_value[4];
    };


    inline Vector4 operator*(const Matrix4x4& m, const Vector4 v)
    {
        Vector4 u;
        u[0] = m[0]  * v[0] + m[1]  * v[1] + m[2]  * v[2] + m[3]  * v[3];
        u[1] = m[4]  * v[0] + m[5]  * v[1] + m[6]  * v[2] + m[7]  * v[3];
        u[2] = m[8]  * v[0] + m[9]  * v[1] + m[10] * v[2] + m[11] * v[3];
        u[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];
        return u;
    }



}


#endif // !GEOMETRY_OBJECT3D_HPP
