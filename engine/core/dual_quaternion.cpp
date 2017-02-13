#include "dual_quaternion.h"
#include "matrix_utils.h"

using namespace Engine;


//TODO
//this doesn't work because of accessing private members of
//Quaternion
/*
void DualQuaternion::fromRotationTranslation (const Quaternion& q, const vec3& trans) {
  // just copy quaternion values
  _w = q._w;
  _x = q._x;
  _y = q._y;
  _z = q._z;

  
  dw = -0.5 * (trans.x * _x + trans.y * _y + trans.z * _z);
  dx = 0.5 * (trans.x * _w + trans.y * _z - trans.z * _y);
  dy = 0.5 * (-trans.x * _z + trans.y * _w + trans.z * _x);
  dz = 0.5 * (trans.x * _y - trans.y * _x + trans.z * _w);
}
*/

void DualQuaternion::fromMatrix(mat4& rot) {

  DecomposedType result;
  
  bool decomposed = decompose(rot, result);

  const Quaternion q = Quaternion(result.quatX, result.quatY, result.quatZ, result.quatW);

  const vec3 pos = glm::vec3(result.transX, result.transY, result.transZ);

  fromRotationTranslation(q, pos);
  
}
