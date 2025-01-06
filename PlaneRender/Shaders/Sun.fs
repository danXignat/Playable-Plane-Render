#version 330 core
out vec4 FragColor;
uniform vec3 lightColor; // Sun color
void main() {
    FragColor = vec4(lightColor, 1.0); // Pure emissive color
}
