#include "Clouds.h"

glm::vec3 GetRandomPos(glm::vec3 position, int horizontalRadius, int verticalRadius, float sky)
{
    srand(time(0));
    float randomY;
    while (true) {
        randomY = position[1] - verticalRadius + float(rand() % verticalRadius * 2);
        if (randomY > sky)
            break;
    }
    float randomX = position[0] - horizontalRadius + float(rand() % (horizontalRadius * 2));
    float randomZ = position[2] - horizontalRadius + float(rand() % (horizontalRadius * 2));
    return glm::vec3{ randomX, randomY, randomZ };
}

float getDistance(glm::vec3 point1, glm::vec3 point2)
{
    return glm::distance(point1, point2);
}
