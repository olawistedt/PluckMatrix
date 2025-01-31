cmake_minimum_required(VERSION 3.14)
project(TemplateProject VERSION 1.0.0)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES
  TemplateProject.cpp
  TemplateProject.h
  resources/resource.h
)

# create a private library target called "__${PROJECT_NAME}-base" for shared code
add_library(_${PROJECT_NAME}-base INTERFACE)
iplug_add_target(_${PROJECT_NAME}-base INTERFACE
  INCLUDE ${PROJECT_DIR} ${PROJECT_DIR}/resources
  LINK iPlug2::IPlug
)

# create an executable target called "_${PROJECT_NAME}-app"
add_executable(${PROJECT_NAME}-app ${SOURCE_FILES})
iplug_add_target(${PROJECT_NAME}-app PUBLIC 
  LINK iPlug2::APP _${PROJECT_NAME}-base 
  RESOURCE ${RESOURCES}
  DEFINE NO_IGRAPHICS
)
iplug_configure_target(${PROJECT_NAME}-app APP ${PROJECT_NAME})