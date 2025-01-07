#include "Sun.h"

// Constructor
Sun::Sun(float radius, float elevationRange)
    : radius(radius), elevationRange(elevationRange), position(0.0f, elevationRange, -radius), sunModel(nullptr) {
}

// Destructor
Sun::~Sun() {
    if (sunModel) {
        delete sunModel;
        sunModel = nullptr;
    }
}

// Initializes the sun with a 3D model and texture
void Sun::initialize(const std::string& modelPath) {
    sunModel = new Model(modelPath, true); // Load the model with smooth normals
}

// Renders the sun, updates its position based on time, and sets light direction in the shader
void Sun::render(Shader& shader, float currentTime, const glm::vec3 cameraPosition) {
    if (!sunModel) return; // Ensure the model is initialized

    // Normalize time (e.g., map to 0 to 2π for a full day cycle)
    float time = fmod(currentTime, 2.0f * glm::pi<float>());

    // Update position based on time
    float x = radius * cos(time);           // East-west movement
    float y = elevationRange * sin(time);   // Vertical (elevation) movement
    float z = -radius;                      // Fixed depth for simplicity

    position = glm::vec3(x, y, z);

    // Use the shader and set uniforms
    shader.use();

    shader.SetVec3("lightPos", position);                    // Sun's position
    shader.SetVec3("viewPos", cameraPosition);               // Camera position
    shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 0.8f)); // Light color
    shader.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Sun object color

    // Calculate the model matrix for the sun's position
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f)); // Adjust scale as needed
    shader.setMat4("model", modelMatrix);

    // Draw the sun model
    sunModel->Draw(shader);
}

// Sets the orbit radius of the sun
void Sun::setRadius(float radius) {
    this->radius = radius;
}

// Sets the vertical elevation range of the sun
void Sun::setElevationRange(float range) {
    this->elevationRange = range;
}
