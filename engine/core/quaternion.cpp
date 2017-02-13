#include <glm/glm.hpp>
#include <cmath>
#include "quaternion.h"

using namespace Engine;

// since i can't just fucking use the one from the math library...
#define PI 3.1415926

const Quaternion Quaternion::ZERO(0,0,0,0);

/* identity quaternion, no args */
Quaternion::Quaternion() :
  _w(1),
  _x(0),
  _y(0),
  _z(0)
{
}


Quaternion::Quaternion(const double w, const double x, const double y, const double z) :
  _w(w),
  _x(x),
  _y(y),
  _z(z)
{
}

/* quaternion from rotation matrix 
 * shamelessly lifted from ogre.
 * algorithm is in the article
 * "Quaternion Calculus and Fast Animation" 
 * by Ken Shoemake
 * which you will never find
 * ever
 */

Quaternion::Quaternion(const glm::mat3 &rot)
{
  // add diagonal elements of matrix
  float trace = rot[0][0] + rot[1][1] + rot[2][2];
  // trace = 1 + 2(cos(theta))
  float root;
  
  if (trace > 0.0f) {
    
    root = sqrt(trace + 1.0f); // 2w

    // w = sqrt(2 + 2(cos(theta))) * 1/2
    _w = 0.5f*root;

    root = 0.5f/root; // 1/(4w) 

    _x = (rot[2][1] - rot[1][2]) * root;
    _y = (rot[0][2] - rot[2][0]) * root;
    _z = (rot[1][0] - rot[0][1]) * root;
  } else {
    // ENTERING THE MYSTERY ZONE
    // WTF?????
    static size_t s_iNext[3] = { 1, 2, 0 };
    size_t i = 0;
    if (rot[1][1] > rot[0][0])
      i = 1;
    if (rot[2][2] > rot[i][i])
      i = 2;

    size_t j = s_iNext[i];
    size_t k = s_iNext[j];

    // ??????????
    // ?????????????????????
    root = sqrt(rot[i][i] - rot[j][j] - rot[k][k] + 1.0f);
    double* apkQuat[3] = { &_x, &_y, &_z };
    *apkQuat[i] = 0.5f * root;
    root = 0.5f / root;
    _w = (rot[k][j] - rot[j][k]) * root;
    *apkQuat[j] = (rot[j][i] + rot[i][j]) * root;
    *apkQuat[k] = (rot[k][j] + rot[i][k]) * root;
    // ????????????????????
    // ??????????
  }
}

/* quaternion from axis / angle */ 
Quaternion::Quaternion(const double &angle, const glm::vec4 axis) 
{
  // quaternion = cos(A/2) + sin(A/2) * (x*i + y*j + z*k)
  // this makes perfect sense
  // wtf is with the other one
  double halfAngle = 0.5 * angle;
  double sine = sin(halfAngle);
  _w = cos(halfAngle);
  _x = sine * axis.x;
  _y = sine * axis.y;
  _z = sine * axis.z;
}

/* make quaternion from orthonormal axes (check that they're actually orthnormal!) */
Quaternion::Quaternion(const glm::vec3 xAxis, const glm::vec3 yAxis, const glm::vec3 zAxis)
{

}

Quaternion Quaternion::unitInverse() const {
  return Quaternion(_w, -_x, -_y, -_z);
}

Quaternion Quaternion::inverse() const {
  float normalized = _w*_w + _x*_x + _y*_y + _z*_z;
  if (normalized > 0.0) {
    float invertedNorm = 1.0f/normalized;
    return Quaternion::Quaternion(_w*invertedNorm, -_x*invertedNorm, -_y*invertedNorm, -_z*invertedNorm);
  } else {
    // this is defined as a zeroed quaternion..
    return Quaternion::ZERO;
  }
}

double Quaternion::dot ( const Quaternion &q ) const {
  return _w*q._w + _x*q._x + _y*q._y + _z*q._z;
}

// hmm.
bool Quaternion::equals ( const Quaternion &rhs, const double tolerance ) {
  double cos = dot(rhs);
  double angle = acos(cos);

  return (fabs(angle <= tolerance));// ||RealEqual(angle, PI, tolerance));
}

double Quaternion::norm () const {
  return _w*_w + _x*_x + _y*_y + _z*_z;
}

glm::vec3 Quaternion::xAxis() const {
  double Ty = 2.0f * _y;
  double Tz = 2.0f * _z;
  double Twy = Ty * _w;
  double Twz = Tz * _w;
  double Txy = Ty * _x;
  double Txz = Tz * _x;
  double Tyy = Ty * _y;
  double Tzz = Tz * _z;
  
  return glm::vec3(1.0f - (Tyy+Tzz), Txy+Twz, Txz-Twy);
}

glm::vec3 Quaternion::yAxis() const {
  float Tx = 2.0f * _x;
  float Ty = 2.0f * _y;
  float Tz = 2.0f * _z;
  float Twx = Tx * _w; 
  float Twz = Tz * _w; 
  float Txx = Tx * _x; 
  float Txy = Ty * _x; 
  float Tyz = Tz * _y; 
  float Tzz = Tz * _z;

  return glm::vec3(Txy-Twz, 1.0f-(Txx+Tzz), Tyz+Twx);
}

glm::vec3 Quaternion::zAxis() const {
  float Tx = 2.0f * _x;
  float Ty = 2.0f * _y;
  float Tz = 2.0f * _z;
  float Twx = Tx * _w;
  float Twy = Ty * _w;
  float Txx = Tx * _x;
  float Txz = Tz * _x;
  float Tyy = Ty * _y;
  float Tyz = Tz * _y;

  return glm::vec3(Txz+Twy, Tyz-Twx, 1.0f-(Txx+Tyy));
}

Quaternion Quaternion::nlerp(const Quaternion& a, const Quaternion& b, float blend) {
  Quaternion result;
  double dot = a.dot(b);
  float blendI = 1.0f - blend;
  if (dot < 0.0f) {
    Quaternion tempB;
    tempB._w = -b._w;
    tempB._x = -b._x;
    tempB._y = -b._y;
    tempB._z = -b._z;

    result._w = blendI * a._w + blend*tempB._w;
    result._x = blendI * a._x + blend*tempB._x;
    result._y = blendI * a._y + blend*tempB._y;
    result._z = blendI * a._z + blend*tempB._z;
  } else {
    result._w = blendI * a._w + blend*b._w;
    result._x = blendI * a._x + blend*b._x;
    result._y = blendI * a._y + blend*b._y;
    result._z = blendI * a._z + blend*b._z;
  }
  result.norm();
  return result;
}
