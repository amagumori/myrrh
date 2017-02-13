#include "types.h"

namespace Engine {
  // just throwing the vec3 namespace in here for now cuz fuck it

  namespace V3 {
    inline double length(vec3 v) {
      return sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    }

    inline vec3 scale ( vec3 v, double scale) {
      double len = length(v);
      if (len != 0) {
        double l = scale / len;
        v[0] *= l;
        v[1] *= l;
        v[2] *= l;
      }
    }

    inline void combine(const vec3 a, const vec3 b, vec3 result, double aScale, double bScale) {
      result[0] = (aScale * a[0]) + (bScale * b[0]);
      result[1] = (aScale * a[1]) + (bScale * b[1]);
      result[2] = (aScale * a[2]) + (bScale * b[2]);
    }

  }


  typedef struct {
    double scaleX, scaleY, scaleZ;
    double skewXY, skewYZ, skewXZ;
    double quatX, quatY, quatZ, quatW;
    double transX, transY, transZ;
    double perspectiveX, perspectiveY, perspectiveZ, perspectiveW;
  } DecomposedType;

  // helper functions that aren't in GLM
  bool decompose(glm::mat4& mat, DecomposedType& result) {
    glm::dmat4 localMatrix = glm::dmat4(mat);

    // normalize matrix.
    if (localMatrix[3][3] == 0)
      return false;
  
    int i, j;
    for (i=0; i < 4; i++) {
      for (j=0; j < 4; j++) {
        localMatrix[i][j] /= localMatrix[3][3];
      }
    }

    // perspectiveMatrix is used to solve for perspective
    // but also provides an easy way to test singularity of
    // upper component.
    glm::dmat4 perspectiveMatrix = localMatrix;

    for (i=0; i < 3; i++) {
      perspectiveMatrix[i][3] = 0;
    }
    perspectiveMatrix[3][3] = 1;

    if (glm::determinant(perspectiveMatrix)== 0) 
      return false;

    // first, isolate the perspective component.
    // this is the grossest part.

    if (localMatrix[0][3] != 0 || localMatrix[1][3] != 0 || localMatrix[2][3] != 0) {
      // RHS: right hand side of equation.  duh
      glm::dvec4 RHS;
      RHS[0] = localMatrix[0][3];
      RHS[1] = localMatrix[1][3];
      RHS[2] = localMatrix[2][3];
      RHS[3] = localMatrix[3][3];

      // solve equation by inverting perspectiveMatrix
      // and multiplying RHS by that (inverse of perspectiveMatrix)

      glm::dmat4 inversePerspectiveMatrix, transposedInversePerspectiveMatrix;
  
      inversePerspectiveMatrix = glm::inverse(perspectiveMatrix);
      transposedInversePerspectiveMatrix = glm::transpose(inversePerspectiveMatrix);
      
      glm::dvec4 perspectivePoint;

      perspectivePoint = transposedInversePerspectiveMatrix * RHS;

      result.perspectiveX = perspectivePoint[0];
      result.perspectiveY = perspectivePoint[1];
      result.perspectiveZ = perspectivePoint[2];
      result.perspectiveW = perspectivePoint[3];

      // clear the perspective partition of the matrix.
      localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0;
      localMatrix[3][3] = 1;
    } else {
      // no perspective.
      result.perspectiveX = result.perspectiveY = result.perspectiveZ = 0;
      result.perspectiveW = 1;
    }

    // next take care of the translation component.

    result.transX = localMatrix[3][0];
    localMatrix[3][0] = 0;
    result.transY = localMatrix[3][1];
    localMatrix[3][1] = 0;
    result.transZ = localMatrix[3][2];
    localMatrix[3][2] = 0;

    glm::dvec3 row[3], pdum3;

    // now, scale and shear.
    for (i=0; i < 3; i++) {
      row[i][0] = localMatrix[i][0];
      row[i][1] = localMatrix[i][1];
      row[i][2] = localMatrix[i][2];
    }

    // compute X scale factor and normalize first row.

    result.scaleX = glm::length(row[0]);

    // scale the vec3 by 1.0.
    V3::scale(row[0], 1.0);

    // compute XY shear factor and make 2nd row orthogonal
    result.skewXY = glm::dot(row[0], row[1]);
    V3::combine(row[1], row[0], row[1], 1.0, -result.skewXY);

    // compute Yscale and normalize 2nd row.
    result.scaleY = glm::length(row[1]);
    V3::scale(row[1], 1.0);
    result.skewXY /= result.scaleY;

    result.skewXZ = glm::dot(row[0], row[2]);
    V3::combine(row[2], row[0], row[2], 1.0, -result.skewXZ);
    result.skewYZ = glm::dot(row[1], row[2]);
    V3::combine(row[2], row[1], row[2], 1.0, -result.skewYZ);

    // now get Zscale and normalize 3rd row.

    result.scaleZ = glm::length(row[2]);
    V3::scale(row[2], 1.0);
    result.skewXZ /= result.scaleZ;
    result.skewYZ /= result.scaleZ;

    // at this point the matrix stored in rows[3] is orthonormal.
    // check for coordinate system flip with determinant.
    // if det = -1, negate the matrix and scaling factors.
    pdum3 = glm::cross(row[1], row[2]);
    if (glm::dot(row[0], pdum3) < 0){
      for (i=0; i < 3; i++) {
        result.scaleX *= -1;
        row[i][0] *= -1;
        row[i][1] *= -1;
        row[i][2] *= -1;
      }
    }

    // now get the final component, rotations, out.

    double s, t, x, y, z, w;

    t = row[0][0] + row[1][1] + row[2][2] + 1.0;
    
    if (t > 1e-4) {
      s = 0.5 / sqrt(t);
      w = 0.25 / s;
      x = (row[2][1] - row[1][2]) * s;
      y = (row[0][2] - row[2][0]) * s;
      z = (row[1][0] - row[0][1]) * s;
    } else if (row[0][0] > row[1][1] && row[0][0] > row[2][2]) {
      s = sqrt(1.0 + row[0][0] - row[1][1] - row[2][2]) * 2.0;
      x = 0.25 * s;
      y = (row[0][1] + row[1][0]);
      z = (row[0][2] + row[2][0]);
      w = (row[2][1] + row[1][2]);
    } else if (row[1][1] > row[2][2]) {
      s = sqrt(1.0 + row[1][1] - row[0][0] - row[2][2]) * 2.0;
      x = (row[0][1] + row[1][0]) / s;
      y = 0.25 * s;
      z = (row[1][2] + row[2][1]) / s;
      w = (row[0][2] + row[2][0]) / s;
    } else {
      s = sqrt(1.0 + row[2][2] - row[0][0] - row[1][1]) * 2.0;
      x = (row[0][2] + row[2][0]) / s;
      y = (row[1][2] + row[2][1]) / s;
      z = 0.25 * s;
      w = (row[1][0] - row[0][1]) / s;
    }

    result.quatX = x;
    result.quatY = y;
    result.quatZ = z;
    result.quatW = w;

    return true;

  }
}

