#include <cmath>

/* CUIDADO
 * THIS FILE IS PERFORMANCE SENSITIVE
 * ALL OF THESE THINGS NEED TO HAPPEN FAST
 * OPTIMIZATIONS GO HERE
 */

namespace Engine {
  class Quaternion {
    public:

      /* CONSTRUCTORS */
      Quaternion();
      Quaternion(const double w, const double x, const double y, const double z);
      Quaternion(const glm::mat3 &rot);
      Quaternion(const double &angle, const glm::vec4 axis);
      /* make quaternion from orthonormal axes (check that they're actually orthnormal!) */
      Quaternion(const glm::vec3 xAxis, const glm::vec3 yAxis, const glm::vec3 zAxis);
      ~Quaternion();

      /* MEMBERS */
      static const Quaternion ZERO;
      static const Quaternion IDENTITY;

      Quaternion unitInverse() const;
      Quaternion inverse() const;
      // x, y, z represent a axis of rotation. 
      double norm () const;
      double dot ( const Quaternion &q ) const;
      // this implements checking equality with a tolerance which i don't think
      // you can do by overloading == operator.  because of this.. not overloading
      // == opperator.
      bool equals ( const Quaternion &rhs, const double tolerance );
      inline bool isNotANumber() const {
        return isnan(_x) || isnan(_y) || isnan(_z) || isnan(_w);
      }
      /* returns orthonormal axis defining this quaternion */
      glm::vec3 xAxis() const;
      glm::vec3 yAxis() const;
      glm::vec3 zAxis() const;

      /* interpolations */

      // READ: http://mollyrocket.com/911.txt
      // apparently adding quaternions and then
      // normalizing them is not only fast
      // but not even that bad error-wise.
      // (only for huge rotations / timestep.)

      // normalized linear interpolation.
      // going to use this for all animation.
      // might implement slerp later if i need it.
      // see:
      // http://physicsforgames.blogspot.com/2010/02/quaternions.html
      // http://number-none.com/product/Hacking%20Quaternions/
      // 
      Quaternion nlerp ( const Quaternion& a,
                         const Quaternion& b,
                         float blend );
 
      Quaternion operator + ( const Quaternion &other ) const;
      Quaternion operator - ( const Quaternion &other ) const;
      Quaternion operator * ( const Quaternion &other ) const;

      inline Quaternion& operator = (const Quaternion& other) {
        _w = other._w;
        _x = other._x;
        _y = other._y;
        _z = other._z;
      }
    private:

      // implementing it the slow but simple way first
      double _w, _x, _y, _z;

  };
}
