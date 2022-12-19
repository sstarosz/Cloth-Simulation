#ifndef GEOMETRY_MATRIX4X4_HPP
#define GEOMETRY_MATRIX4X4_HPP

#include <compare>

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


        auto operator<=>(const Matrix4x4&) const = default;


    private:
        float m_value[16];
    };


}


#endif // !GEOMETRY_OBJECT3D_HPP
