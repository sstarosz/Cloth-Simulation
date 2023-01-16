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
    class Vector3D
    {
    public:
        Vector3D() noexcept;
        Vector3D(float x, float y, float z) noexcept;


        constexpr float& operator[](const size_t index)
        {
            assert(index >= 0 && index < 4);
            return ((&x)[index]);   //TODO More safe version?
        }
         
        const float& operator[](const size_t index) const
        {
            assert(index >= 0 && index < 4);
            return ((&x)[index]); //TODO More safe version?
        }

        auto operator<=>(const Vector3D&) const = default;


        Vector3D operator+(const Vector3D& vec) const
        {
            return {x + vec.x, y + vec.y, z + vec.z};
        }

        Vector3D operator-(const Vector3D& vec) const
        {
            return {x - vec.x, y - vec.y, z - vec.z};
        }

        //Vector3D operator*(const float& s) const
        //{
		//    return {x * s, y * s, z * s};
	    //}


        Vector3D& operator*=(const float& s)
        {
            x *= s;
            y *= s;
            z *= s;
            return *this;
	    }

        Vector3D& operator/=(const float& s)
        {
            assert(s != 0.0F);
            const float ms = 1.0F / s;
            x *= ms;
            y *= ms;
            z *= ms;
            return *this;
	    }
       

        static float lenght(const Vector3D& vec);
        static Vector3D normalize(const Vector3D& vec);
        static Vector3D crossProduct(const Vector3D& vec, const Vector3D& w);

    public:
        float x;
        float y;
        float z;
    };



    inline Vector3D operator*(const Vector3D& vec, float s)
    {
        return {vec.x * s, vec.y * s, vec.z * s};
    }

    inline Vector3D operator/(const Vector3D& vec, float s)
    {
        assert(s != 0.0F);
        const float ms = 1.0F / s;
        return {vec.x * ms, vec.y * ms, vec.z * ms};
    }

    inline Vector3D operator-(const Vector3D& vec)
    {
        return {-vec.x, -vec.y, -vec.z};
    }
}


#endif // !GEOMETRY_OBJECT3D_HPP
