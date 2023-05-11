// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

#include <array>

#include "polyscope/camera_parameters.h"
#include "polyscope/gl/gl_utils.h"

// GLM for view matrices
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/norm.hpp" // necessary for dual_quaternion below
#undef GLM_ENABLE_EXPERIMENTAL

namespace polyscope {
namespace view {

enum class NavigateStyle { Turntable = 0, Free, Planar, Arcball };

// === View state
extern int bufferWidth;
extern int bufferHeight;
extern int windowWidth;
extern int windowHeight;
extern int initWindowPosX;
extern int initWindowPosY;
extern NavigateStyle style;
extern double moveScale;
extern double nearClipRatio;
extern double farClipRatio;
extern std::array<float, 4> bgColor;

// Current view camera parameters
extern glm::mat4x4 viewMat;
extern double fov; // in the y direction

// "Flying" view
extern bool midflight;
extern float flightStartTime;
extern float flightEndTime;
extern glm::dualquat flightTargetViewR, flightInitialViewR;
extern glm::vec3 flightTargetViewT, flightInitialViewT;
extern float flightTargetFov, flightInitialFov;

// Default values
extern const double defaultNearClipRatio;
extern const double defaultFarClipRatio;
extern const double defaultFov;

// === View methods

void processTranslate(glm::vec2 delta);
void processRotate(glm::vec2 startP, glm::vec2 endP);

void processClipPlaneShift(double amount);
void processZoom(double amount);

void setWindowSize(int width, int height);
void setViewToCamera(const CameraParameters& p);

// Invlidateing the view
// The view is invalid if the viewMat has NaN entries.
// It is set to invalid initially, but we call ensureViewValid() before any renders.
// This ensures we never try to redner with an invalid view, but also allows the user to
// set custom views if they wish, without them getting overwritten.
void invalidateView();
void ensureViewValid();

// The "default" view simply looks at the origin
void resetCameraToDefault();
void flyToDefault();

// The "home" view looks at the center of the scenes bounding box.
glm::mat4 computeHomeView();
void resetCameraToHomeView();
void flyToHomeView();

glm::mat4 getCameraViewMatrix();
glm::mat4 getCameraPerspectiveMatrix();
glm::vec3 getCameraWorldPosition();

void getCameraFrame(glm::vec3& lookDir, glm::vec3& upDir, glm::vec3& rightDir);

// Flight-related
void startFlightTo(const CameraParameters& p, float flightLengthInSeconds = .25);
void startFlightTo(const glm::mat4& T, float targetFov, float flightLengthInSeconds = .25);
void immediatelyEndFlight();
void splitTransform(const glm::mat4& trans, glm::mat3x4& R, glm::vec3& T);
glm::mat4 buildTransform(const glm::mat3x4& R, const glm::vec3& T);
void updateFlight();

} // namespace view
} // namespace polyscope
