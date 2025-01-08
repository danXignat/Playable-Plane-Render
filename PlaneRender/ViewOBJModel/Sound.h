#pragma once
#include "Includes.h"

class Sound
{
public:
	static std::string ForestPath;
	static std::string JetPath;
	static std::string ExplosionPath;
	static void playMusic(std::string rootPath,std::string soundPath);
	static void stopMusic();
};

