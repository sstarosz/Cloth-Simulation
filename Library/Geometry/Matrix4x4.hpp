#ifndef GEOMETRY_MATRIX4X4_HPP
#define GEOMETRY_MATRIX4X4_HPP

#include <compare>
#include "Vector3.hpp"

namespace st::geometry
{

    /*! \brief Point in 3d space
         *         Brief description continued.
         *
         *  Detailed description starts here.
         */
    class Matrix4x4
    {
    public:
        Matrix4x4() noexcept;
        Matrix4x4(float xa, float xb, float xc, float xd,
                    float ya, float yb, float yc, float yd,
                    float za, float zb, float zc, float zd,
                    float wa, float wb, float wc, float wd) noexcept;




        Matrix4x4& operator*=(const Matrix4x4& rhs)
        {
            m_value[0] = m_value[0] * rhs[0] + m_value[1] * rhs[4] + m_value[2] * rhs[8]  + m_value[3] * rhs[12];
            m_value[1] = m_value[0] * rhs[1] + m_value[1] * rhs[5] + m_value[2] * rhs[9]  + m_value[3] * rhs[13];
            m_value[2] = m_value[0] * rhs[2] + m_value[1] * rhs[6] + m_value[2] * rhs[10] + m_value[3] * rhs[14];
            m_value[3] = m_value[0] * rhs[3] + m_value[1] * rhs[7] + m_value[2] * rhs[11] + m_value[3] * rhs[15];

            m_value[4] = m_value[4] * rhs[0] + m_value[5] * rhs[4] + m_value[6] * rhs[8]  + m_value[7] * rhs[12];
            m_value[5] = m_value[4] * rhs[1] + m_value[5] * rhs[5] + m_value[6] * rhs[9]  + m_value[7] * rhs[13];
            m_value[6] = m_value[4] * rhs[2] + m_value[5] * rhs[6] + m_value[6] * rhs[10] + m_value[7] * rhs[14];
            m_value[7] = m_value[4] * rhs[3] + m_value[5] * rhs[7] + m_value[6] * rhs[11] + m_value[7] * rhs[15];

            m_value[8]  = m_value[8] * rhs[0] + m_value[9] * rhs[4] + m_value[10] * rhs[8]  + m_value[11] * rhs[12];
            m_value[9]  = m_value[8] * rhs[1] + m_value[9] * rhs[5] + m_value[10] * rhs[9]  + m_value[11] * rhs[13];
            m_value[10] = m_value[8] * rhs[2] + m_value[9] * rhs[6] + m_value[10] * rhs[10] + m_value[11] * rhs[14];
            m_value[11] = m_value[8] * rhs[3] + m_value[9] * rhs[7] + m_value[10] * rhs[11] + m_value[11] * rhs[15];

            m_value[12] = m_value[12] * rhs[0] + m_value[13] * rhs[4] + m_value[14] * rhs[8]  + m_value[15] * rhs[12];
            m_value[13] = m_value[12] * rhs[1] + m_value[13] * rhs[5] + m_value[14] * rhs[9]  + m_value[15] * rhs[13];
            m_value[14] = m_value[12] * rhs[2] + m_value[13] * rhs[6] + m_value[14] * rhs[10] + m_value[15] * rhs[14];
            m_value[15] = m_value[12] * rhs[3] + m_value[13] * rhs[7] + m_value[14] * rhs[11] + m_value[15] * rhs[15];


            return *this;
        }

        constexpr float& operator[](const size_t index)
        {
            assert(index >= 0 && index <= 15);
            return m_value[index];
        }

        const float& operator[](const size_t index) const
        {
            assert(index >= 0 && index <= 15);
            return m_value[index];
        }

        auto operator<=>(const Matrix4x4&) const = default;


        static Matrix4x4 indentityMatrix();
        void translate(const Vector3 vector);

        static Matrix4x4 projectionMatrix(float fieldOfView, float framebufferAspectRatio, float nearPlane, float farPlane);



        static Matrix4x4 rotationX(const float& theta);
        static Matrix4x4 rotationY(const float& theta);
        static Matrix4x4 rotationZ(const float& theta);
        static Matrix4x4 rotationAroundAxis(const float& theta, const Vector3& v);


        void convertToColumnMajor();

    private:
        float m_value[16];
    };

}



#endif // !GEOMETRY_OBJECT3D_HPP
