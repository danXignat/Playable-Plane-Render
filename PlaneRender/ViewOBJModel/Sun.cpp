#include "Sun.h"

// Constructor
Sun::Sun(float radius, float elevationRange)
    : radius(radius), elevationRange(elevationRange), position(0.0f, elevationRange, -radius), sunModel(nullptr), realTime(12.0f) {  // Default to 12:00 PM
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

// Sets the real-time for sun position calculation (24-hour format)
void Sun::setRealTime(float hours) {
    this->realTime = fmod(hours, 24.0f);  // Ensure the time wraps correctly (e.g., 25 becomes 1)
}

// Gets the current real-world time in hours (24-hour format)
float Sun::getRealTimeInHours() const {
    // Get the current time from the system
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    struct tm local_time;
    localtime_s(&local_time, &now_time);  // Get local time as a struct tm

    // Extract the hour part (24-hour format)
    return static_cast<float>(local_time.tm_hour) + static_cast<float>(local_time.tm_min) / 60.0f;
}

// Renders the sun, updates its position based on real time, and sets light direction in the shader
void Sun::render(Shader& shader, float realTime, const glm::vec3 cameraPosition) {
    if (!sunModel) return; // Ensure the model is initialized

    // Set the time (in case it's not passed as argument)
    setRealTime(realTime);

    // Normalize the time to be between 0 and 2 * pi for full circle movement
    float time = glm::radians((realTime-4) / 24.0f * 360.0f);  // Mapping 24-hour format to angle in a day cycle

    // Calculate sun's position in the sky
    float x = radius * cos(time-90);           // East-west movement
    float y = elevationRange * sin(time-90);   // Vertical (elevation) movement
    float z = -radius * cos(time-90);         // Fixed depth (radius-dependent) for simplicity

    position = glm::vec3(x, y, z);

    // Use the shader and set uniforms
    shader.use();

    shader.SetVec3("lightPos", position);                    // Sun's position
    shader.SetVec3("viewPos", cameraPosition);               // Camera position
   // shader.SetVec3("lightColor", glm::vec3(1.0f, 0.7f, 0.4f)); // Light color sunset
    shader.SetVec3("lightColor", glm::vec3(0.98, 0.92, 0.80));
    shader.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Sun object color
    //shader.setFloat("diffuseStrength", 10.0f);

    // Calculate the model matrix for the sun's position
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(30)); // Adjust scale as needed
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.0, 1.0,0.0));
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
