#pragma once
#include "Includes.h"
#include "Model.h"
#include <memory>

class MovingPlane {
private:
	std::shared_ptr<Model> planeObjModel;

public:

	MovingPlane(const std::string& _path);

};