#ifndef _DUALQUATERNION_H
#define _DUALQUATERNION_H

#include "types.h"
#include "quaternion.h"
#include <iostream>

// this whole class is basically lifted from OGRE.

namespace Engine {
  class DualQuaternion {
    public:
      inline DualQuaternion() :
        _w(1), _x(0), _y(0), _z(0), _dw(1), _dx(0), _dy(0), _dz(0)
      {
      }

      inline DualQuaternion(mat4& rot) {
        this->fromMatrix(rot);
      }

      inline DualQuaternion(const Quaternion& q, const vec3& trans) {
        this->fromRotationTranslation(q, trans);
      } 

      inline DualQuaternion& operator = (const DualQuaternion& other) {
        _w = other._w;
        _x = other._x;
        _y = other._y;
        _z = other._z;
        _dw = other._dw;
        _dx = other._dx;
        _dy = other._dy;
        _dz = other._dz;
      }

      void fromRotationTranslation(const Quaternion& q, const vec3& trans);
      // easier to make this not const for now?
      void fromMatrix(mat4& rot);
     
      // helper function for writing a dual quaternion to ostream.
      inline friend std::ostream& operator << (std::ostream& o, const DualQuaternion& q) {
        o << "Dual Quaternion: [" << q._w << ", " << q._x << ", " << q._y << ", " << q._z << ", " << q._dw << ", " << q._dx << ", " << q._dy << ", " << q._dz << "]\n";
        return o;
      }


    private:
      float _w, _x, _y, _z, _dw, _dx, _dy, _dz;
  };
}

#endif
