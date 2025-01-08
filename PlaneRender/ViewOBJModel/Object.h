#pragma once
#include "Model.h"
#include "Camera.h"

class Object {
public:
	Object(const std::string& path, Camera& pCamera);

	virtual void render() = 0;

protected:
	Camera& pCamera;
	std::string path;
};