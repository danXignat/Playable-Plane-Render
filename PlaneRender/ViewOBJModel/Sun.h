#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Sun {
public:
    // Constructor
    Sun(float radius = 100.0f, float elevationRange = 50.0f);

    // Initializes the sun with a 3D model and texture
    void initialize(const std::string& modelPath);

    // Updates the sun's position based on time
    void updatePosition(float time);

    // Draws the sun using the provided shader
    void draw(Shader& shader);

    // Retrieves the sun's directional light vector (normalized)
    glm::vec3 getLightDirection() const;

    // Sets the orbit radius of the sun
    void setRadius(float radius);

    // Sets the vertical elevation range of the sun
    void setElevationRange(float range);

private:
    float radius;          // Radius of the sun's orbit
    float elevationRange;  // Maximum height (elevation) of the sun
    glm::vec3 position;    // Current position of the sun
    Model* sunModel;       // Model used to represent the sun
};
