#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "Model.h"
#include <chrono>
#include <ctime>

class Sun {
public:
    // Constructor
    Sun(float radius = 100.0f, float elevationRange = 50.0f);

    // Destructor
    ~Sun();

    // Initializes the sun with a 3D model and texture
    void initialize(const std::string& modelPath);

    // Renders the sun, updates its position based on real time, and sets light direction in the shader
    void render(Shader& shader, float realTime, glm::vec3 cameraPosition);

    // Sets the orbit radius of the sun
    void setRadius(float radius);

    // Sets the vertical elevation range of the sun
    void setElevationRange(float range);

    // Sets the real-time for sun position calculation
    void setRealTime(float hours);  // Time in hours (24-hour format)

    // Gets the current real-world time in hours (24-hour format)
    float getRealTimeInHours() const;

private:
    float radius;          // Radius of the sun's orbit
    float elevationRange;  // Maximum height (elevation) of the sun
    glm::vec3 position;    // Current position of the sun
    Model* sunModel;       // Model used to represent the sun
    float realTime;        // Real-world time (in hours) for sun position
};
