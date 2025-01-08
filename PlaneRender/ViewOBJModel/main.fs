#version 330 core
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;    

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth + 0.005 ? 1.0 : 0.0;
    return shadow;
}

void main() {
    vec3 color = vec3(1.0); 
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 shadowColor = (1.0 - shadow) * color;

    FragColor = vec4(shadowColor, 1.0);
}