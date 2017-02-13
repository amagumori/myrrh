#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <list>

#include "../include/Platform.h"
#include "../include/Program.h"
#include "../include/Bitmap.h"
#include "../include/Texture.h"
#include "../include/Mesh.h" // this includes Program.h which includes Shader.h?
#include "../include/Lights.h"
#include "../include/Camera.h"

const glm::vec2 SCREEN_SIZE(800, 600);

double gScrollY = 0.0;
GLfloat gDegreesRotated = 0.0;
GLFWwindow* gWindow = NULL;
Engine::Program* gProgram = NULL;
Engine::Camera gCamera;
GLuint gVAO = 0;
GLuint gVBO = 0;

Engine::ModelAsset girugaMesh;

// containers for lights and models.
std::list<Engine::ModelInstance> gInstances;
std::vector<Engine::Light> gLights;

static void Render ( ) {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(gProgram->object());
  
  glBindVertexArray(gVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

  glUseProgram(0);
  glfwSwapBuffers(gWindow);
}

static Engine::Program *loadShaders(const char *vert_shader, const char *frag_shader) {
  std::vector<Engine::Shader> shaders;
  shaders.push_back(Engine::Shader::shaderFromFile(ResourcePath(vert_shader), GL_VERTEX_SHADER));
  shaders.push_back(Engine::Shader::shaderFromFile(ResourcePath(frag_shader), GL_FRAGMENT_SHADER));
  return new Engine::Program(shaders);
}

static Engine::Texture *loadTexture(const char *filename) {
  Engine::Bitmap bmp = Engine::Bitmap::bitmapFromFile(ResourcePath(filename));
  bmp.flipVertically();
  return new Engine::Texture(bmp); 
}

static void loadTestMesh() {
  girugaMesh.shaders = loadShaders("vertex-shader.txt", "fragment-shader.txt");
  girugaMesh.draw_type = GL_TRIANGLES;
  girugaMesh.draw_start = 0;
  girugaMesh.draw_count = 0;
  girugaMesh.texture = loadTexture("tex.jpg");
  girugaMesh.shininess = 80.0;
  girugaMesh.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
  glGenBuffers(1, &girugaMesh.vbo);
  glGenVertexArrays(1, &girugaMesh.vao);

  glBindVertexArray(girugaMesh.vao);
  glBindBuffer(1, girugaMesh.vbo);

  std::vector<GLfloat> vertexData = girugaMesh.vertices;

  // unbind VAO
  glBindVertexArray(0);
}

static void CreateInstances() {
  // for now just gonna create one.
  Engine::ModelInstance fooMesh;
  fooMesh.asset = &girugaMesh;
  fooMesh.transform = glm::mat4();
  gInstances.push_back(fooMesh);
}

static void Update(float secondsElapsed) {
  const GLfloat degreesPerSecond = 180.0f;
  gDegreesRotated += secondsElapsed * degreesPerSecond;
  while ( gDegreesRotated > 360.0f ) gDegreesRotated -= 360.0f;
  gInstances.front().transform = glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0));
  
  const float moveSpeed = 4.0;
  if (glfwGetKey(gWindow, 'S')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward()); 
  } else if (glfwGetKey(gWindow, 'A')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right()); 
  } else if (glfwGetKey(gWindow, 'W')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward()); 
  } else if (glfwGetKey(gWindow, 'D')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right()); 
  }

  if (glfwGetKey(gWindow, 'Z')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0,1,0));
  } else if (glfwGetKey(gWindow, 'X')) {
    gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0,1,0));
  }

  if (glfwGetKey(gWindow, '1')) {
    gLights[0].position = glm::vec3(gCamera.position());
  }

  if (glfwGetKey(gWindow, '2')) {
    gLights[0].color = glm::vec3(2,0,0);
  } else if (glfwGetKey(gWindow, '3')) {
    gLights[0].color = glm::vec3(0,2,0);
  } else if (glfwGetKey(gWindow, '4')) {
    gLights[0].color = glm::vec3(2,2,2);
  }

  const float mouseSensitivity = 0.1f;
  double mouseX, mouseY;
  glfwGetCursorPos(gWindow, &mouseX, &mouseY);
  gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
  glfwSetCursorPos(gWindow, 0, 0);

  const float zoomSensitivity = -0.2f;
  float fieldOfView = gCamera.fieldOfView() + zoomSensitivity * (float)gScrollY;
  if (fieldOfView < 5.0f) fieldOfView = 5.0f;
  if (fieldOfView > 130.0f) fieldOfView = 130.0f;
  gCamera.setFieldOfView(fieldOfView);
  gScrollY = 0;
}

void OnScroll (GLFWwindow *window, double deltaX, double deltaY) {
  gScrollY += deltaY;
}


void OnError (int errorCode, const char *msg) {
  throw std::runtime_error(msg);
}

// do all the glfw init bullshit and etc

void AppMain () {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "test window", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwSetScrollCallback(gWindow, OnScroll);
    glfwMakeContextCurrent(gWindow);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");

    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialise the test asset
    loadTestMesh();

    // create all the instances in the 3D scene based on the gWoodenCrate asset
    CreateInstances();

    // setup gCamera
    gCamera.setPosition(glm::vec3(-4,0,17));
    // fuck dis gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);

    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;

        // draw one frame
        Render();

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }

    // clean up and exit
    glfwTerminate();
}

int main (int argc, char **argv ) {

  try {
    AppMain();
  } catch ( const std::exception &e ) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::cout << "fuck you\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

  return 0;
}

