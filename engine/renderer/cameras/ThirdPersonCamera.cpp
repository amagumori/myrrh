#include "../include/ThirdPersonCamera.h"

using namespace engine;
/*
 * thoughts:
 *
 * QUATERNIONS, or even DUAL QUATERNIONS!!1
 *
 * want to define a minimum distance the camera will remain from the player
 * for not terrible way-too-zoomed-in camera. call this minDistanceFromPlayer.
 * when solid geometry lies between camera and player:
 *
 *  figure this out by z-test in camera's local (model) coordinate space,
 *  because camera's Z will always be pointing at player's back.
 *
 *  (if you create a depth texture / fbo for the camera then z-test against this,
 *   it will happen after shading so the fix of the camera's position will start
 *   happening after the frame it's "realized" on)
 *
 * then:
 *  either zoom in to a distance > depth of occluding geometry but strictly < minDistanceFromPlayer
 *    (e.g. if occluding geo is at Z=56.0 then zoom to 56.0 + EPSILON where EPSILON is small)
 *  or, just clip all occluding polygons
 *
 *  turns out this is kinda the most naive of 3rd person cameras.  but.  w/e.
 *  
 *  then, how to handle motion of the camera?
 *
 *  "cinematic" cameras are done with 3d b-splines and motion planning on navmeshes.
 *  jesus fucking christ.
 *  nothing is ever easy....
 */
void ThirdPersonCamera::setRadius ( float rad ) {
  _radius = rad;
}

void ThirdPersonCamera::resetCamera () {
  _horizontalAngle = foobar;
  _verticalAngle = foobar;
  printf("what the fuck");
}

void ThirdPersonCamera::rotateCamera ( float yRot, float xRot ) {

}

