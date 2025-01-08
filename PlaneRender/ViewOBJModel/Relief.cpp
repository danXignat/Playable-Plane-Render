#include "Relief.h"

Relief::Relief(const std::string path) :
    objModel{ path + "\\Models\\Relief\\untitled.obj", false },
    ball{ path + "\\Models\\Ball\\source\\FreeStone Sphere.obj", false }
{
    model = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    model = glm::translate(model, glm::vec3(0.f, -2300.f, -150.f));

    _parseFile();
}

void Relief::render() {
    utils::DrawModel(MainWindow::instance().sunShader, model, objModel);

     /*for (const auto& vec : testBall) {
         glm::mat4 ballModel = glm::translate(glm::mat4(1.0), vec);
         ballModel = glm::scale(ballModel, glm::vec3(4.f));
         utils::DrawModel(MainWindow::instance().sunShader, ballModel, ball);
     }*/
}

void Relief::_parseFile() {
    std::string filename{ "relief_vertices.txt" };
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        float num1, num2, num3;

        if (lineStream >> num1 >> num2 >> num3) {
            glm::vec3 point{ num1, num2, num3 };

            glm::vec4 transformedPoint = model * glm::vec4(point, 1.0f);

            glm::vec3 transformedVec3 = glm::vec3(transformedPoint);

            //testBall.push_back(transformedVec3);

            kdTree.insert(transformedVec3);
        }
        else {
            std::cerr << "Warning: Malformed line encountered, skipping: " << line << std::endl;
        }
    }

    file.close();
}