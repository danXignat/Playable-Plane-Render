#include "Sun.h"

// Constructor
Sun::Sun(float radius, float elevationRange)
    : radius(radius), elevationRange(elevationRange), position(0.0f, elevationRange, -radius), sunModel(nullptr) {
}

// Initializes the sun with a 3D model and texture
void Sun::initialize(const std::string& modelPath) {
    sunModel = new Model(modelPath, true); // Load the model with smooth normals
}

// Updates the sun's position based on time
void Sun::updatePosition(float time) {
    // Time should ideally be normalized (e.g., 0 to 2π for a full day cycle)
    float angle = fmod(time, 2.0f * glm::pi<float>()); // Normalize to [0, 2π]

    // Calculate position in the sky using trigonometric functions
    float x = radius * cos(angle);           // East-west movement
    float y = elevationRange * sin(angle);   // Vertical (elevation) movement
    float z = -radius;                       // Fixed depth for simplicity

    position = glm::vec3(x, y, z);
}

// Draws the sun using the provided shader
void Sun::draw(Shader& shader) {
    if (!sunModel) return; // Ensure the model is initialized

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f)); // Adjust scale as needed

    shader.use();
    shader.setMat4("model", modelMatrix);
    sunModel->Draw(shader);
}

// Retrieves the sun's directional light vector (normalized)
glm::vec3 Sun::getLightDirection() const {
    // Direction is inverted to point from the sun to the origin
    return glm::normalize(-position);
}

// Sets the orbit radius of the sun
void Sun::setRadius(float radius) {
    this->radius = radius;
}

// Sets the vertical elevation range of the sun
void Sun::setElevationRange(float range) {
    this->elevationRange = range;
}
