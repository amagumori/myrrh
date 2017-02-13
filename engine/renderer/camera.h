#pragma once

#include <glm/glm.hpp>

namespace Engine {
  class Camera {
    public:
      Camera();

      /* position */
      const glm::vec3& position() const;
      void setPosition(const glm::vec3 position);
      void offsetPosition(const glm::vec3 offset);

      float fieldOfView() const;
      void setFieldOfView(float fieldOfView);

      float nearPlane() const;
      float farPlane() const;
      void setNearAndFarPlanes(float nearPlane, float farPlane);

      glm::mat4 orientation() const; 
      void offsetOrientation(float upAngle, float rightAngle);

      void lookAt(glm::vec3 position);

      float viewportAspectRatio() const;
 
      /* unit vectors */ 
      glm::vec3 forward() const;
      glm::vec3 right() const;
      glm::vec3 up() const;

      /* combined camera transform matrix, including perspective
       * projection.  this is what goes to vertex shader. */
      glm::mat4 matrix() const;
      /* perspective projection matrix */
      glm::mat4 projection() const;
      /* translation and rotation matrix of camera.
       * i.e. not perspective projected. */
      glm::mat4 view() const;
    private:
      glm::vec3 _position;
      float _horizontalAngle;
      float _verticalAngle;
      float _fieldOfView;
      float _nearPlane;
      float _farPlane;
      float _viewportAspectRatio;
      void normalizeAngles();
  };
}
