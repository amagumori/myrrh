cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(ROOT_PATH ${PROJECT_SOURCE_DIR})
set(ENGINE_PATH ${ROOT_PATH}/engine/)

## core files ##

file(GLOB CORE_FILES ${ENGINE_PATH}/core/*.cpp ${ENGINE_PATH}/core/*.h)
file(GLOB CORE_ECS_FILES  ${ENGINE_PATH}/core/ECS/*.cpp ${ENGINE_PATH}/core/ECS/*.h)
file(GLOB CORE_FS_FILES ${ENGINE_PATH}/core/fs/*.cpp ${ENGINE_PATH}/core/fs/*.h)
file(GLOB CORE_RESOURCES_FILES ${ENGINE_PATH}/core/resources/*.cpp ${ENGINE_PATH}/core/resources/*.h)

## engine files ##

file(GLOB ENGINE_FILES ${ENGINE_PATH}/*.cpp ${ENGINE_PATH}/*.h)

## renderer files ##

file(GLOB RENDERER_FILES ${ENGINE_PATH}/renderer/*.cpp ${ENGINE_PATH}/renderer/*.h)

## animation files ##

file(GLOB ANIMATION_FILES ${ENGINE_PATH}/animation/*.cpp ${ENGINE_PATH}/animation/*.h)

include_directories(${ROOT_PATH})
include_directories(${ENGINE_PATH})
include_directories(${ENGINE_PATH})

add_library(engine SHARED
  ${CORE_FILES}
  ${CORE_ECS_FILES}
  ${CORE_FS_FILES}
  ${CORE_RESOURCES_FILES}
  ${RENDERER_FILES}
  ${ANIMATION_FILES}
  ${ENGINE_FILES}
)
