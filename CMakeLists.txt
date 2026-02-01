cmake_minimum_required(VERSION 3.21)

project(BLHL
  VERSION 0.1.0
  LANGUAGES CXX
)

# ----------------------------------
# Options
# ----------------------------------
option(BLHL_BUILD_TESTS "Build BLHL test executables" OFF)
option(BLHL_FETCH_DEPS  "Fetch dependencies locally (dev mode)" OFF)
option(BLHL_BUILD_LIB   "Build the BLHL library" ON)

# ----------------------------------
# C++ standard
# ----------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------
# Library
# ----------------------------------
if(BLHL_BUILD_LIB)
  file(GLOB BLHL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/BLHL_UI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/.headers/*.cpp
  )

  add_library(BLHL ${BLHL_SOURCES})
  add_library(BLHL::BLHL ALIAS BLHL)

  target_include_directories(BLHL
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.headers>
      $<INSTALL_INTERFACE:include>
  )
endif()

# ----------------------------------
# Dependencies
# ----------------------------------
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

if (BLHL_FETCH_DEPS)
  include(FetchContent)

  FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
  FetchContent_Declare(glm  GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.3)
  FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.92.5)
  FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v2.0.8)

  FetchContent_MakeAvailable(glfw glm imgui glad)
else()
  find_package(glfw3 CONFIG REQUIRED)
  find_package(glm CONFIG REQUIRED)
  find_package(imgui CONFIG REQUIRED)
  find_package(glad CONFIG REQUIRED)
endif()

if(BLHL_BUILD_LIB)
  target_link_libraries(BLHL
    PUBLIC
      Vulkan::Vulkan
      OpenGL::GL
      glfw
      glm::glm
      imgui::imgui
      glad::glad
  )
endif()

# ----------------------------------
# Optional TESTS
# ----------------------------------
if (BLHL_BUILD_TESTS)
  #================================================================#

  set(VK_TEST_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Tests/build/vulkan)
  file(MAKE_DIRECTORY ${VK_TEST_OUTPUT_DIR})
  file(GLOB VK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/.headers/vulkan/*.cpp)
  

  add_executable(BLHL_vk_test Tests/VK_TEST.cpp ${VK_SOURCES})
  set_target_properties(BLHL_vk_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${VK_TEST_OUTPUT_DIR}
  )

  target_include_directories(BLHL_vk_test 
    PRIVATE
      ".headers"
  )
  target_link_libraries(BLHL_vk_test
    PRIVATE
      $<$<BOOL:${BLHL_BUILD_LIB}>:BLHL::BLHL>
      Vulkan::Vulkan
      glfw
      glm::glm
      imgui::imgui
  )

  #================================================================#
  #================================================================#

  set(GL_TEST_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Tests/build/opengl)
  file(MAKE_DIRECTORY ${GL_TEST_OUTPUT_DIR})
  file(GLOB GL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/.headers/opengl/*.cpp)

  add_executable(BLHL_gl_test Tests/GL_TEST.cpp ${GL_SOURCES})
  set_target_properties(BLHL_gl_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${GL_TEST_OUTPUT_DIR}
  )

  target_include_directories(BLHL_gl_test 
    PRIVATE
      ".headers"
  )
  target_link_libraries(BLHL_gl_test
    PRIVATE
      $<$<BOOL:${BLHL_BUILD_LIB}>:BLHL::BLHL>
      glad::glad
      glfw
      glm::glm
      imgui::imgui
      OpenGL::GL
  )

  #================================================================#
endif()

# ----------------------------------
# Install rules
# ----------------------------------
if(BLHL_BUILD_LIB)
  include(GNUInstallDirs)

  install(
    TARGETS BLHL
    EXPORT BLHLTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(
    DIRECTORY .headers/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT BLHLTargets
    NAMESPACE BLHL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BLHL
  )

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    BLHLConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/BLHLConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/BLHLConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BLHL
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/BLHLConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/BLHLConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BLHL
  )
endif()
