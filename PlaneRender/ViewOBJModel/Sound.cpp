#include "Sound.h"

std::string Sound::ForestPath{ "\\Sounds\\forest-sounds.mp3" };
std::string Sound::ExplosionPath{ "\\Sounds\\explosion.mp3" };
std::string Sound::JetPath{ "\\Sounds\\jet-sound.mp3" };

void Sound::playMusic(std::string rootPath, std::string soundPath)
{
	std::string musicPath = (rootPath + soundPath);

	std::ifstream file(musicPath);
	if (!file) {
		std::cerr << "Error: File does not exist at path: " << musicPath << std::endl;
		return;
	}
	else {
		std::cout << "File found: " << musicPath << std::endl;
	}

	std::wstring wideMusicPath = std::wstring(musicPath.begin(), musicPath.end());
	//std::wstring command = L"open "" + wideMusicPath + L"" type mpegvideo alias bgm";
	std::wstring command = L"open \"" + wideMusicPath + L"\" type mpegvideo alias bgm";

	if (mciSendString(command.c_str(), NULL, 0, NULL) != 0) {
		WCHAR errorBuffer[256];
		mciGetErrorString(GetLastError(), errorBuffer, 256);
		std::wcerr << L"MCI Error: " << errorBuffer << std::endl;
		return;
	}

	if (mciSendString(L"play bgm repeat", NULL, 0, NULL) != 0) {
		WCHAR errorBuffer[256];
		mciGetErrorString(GetLastError(), errorBuffer, 256);
		std::wcerr << L"MCI Error: " << errorBuffer << std::endl;
		return;
	}
}

void Sound::stopMusic()
{
	// Stop the music associated with alias "bgm"
	if (mciSendString(L"stop bgm", NULL, 0, NULL) != 0) {
		WCHAR errorBuffer[256];
		mciGetErrorString(GetLastError(), errorBuffer, 256);
		std::wcerr << L"MCI Error (stop): " << errorBuffer << std::endl;
	}

	// Close the alias "bgm" to release resources
	if (mciSendString(L"close bgm", NULL, 0, NULL) != 0) {
		WCHAR errorBuffer[256];
		mciGetErrorString(GetLastError(), errorBuffer, 256);
		std::wcerr << L"MCI Error (close): " << errorBuffer << std::endl;
	}
}