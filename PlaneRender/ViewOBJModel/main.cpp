#include "MainWindow.h"
#include "KDTree.h"

#include "random"

void playMusic(std::string currentPath)
{
    std::string musicPath = currentPath + "\\Sounds\\forest-sounds.mp3";

    std::ifstream file(musicPath);
    if (!file) {
        std::cerr << "Error: File does not exist at path: " << musicPath << std::endl;
        return;
    }
    else {
        std::cout << "File found: " << musicPath << std::endl;
    }

    std::wstring wideMusicPath = std::wstring(musicPath.begin(), musicPath.end());
    std::wstring command = L"open "" + wideMusicPath + L"" type mpegvideo alias bgm";

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

int main() {
    //Sounds soundEngine();


    MainWindow& window = MainWindow::instance();

    playMusic(window.currentPath);

    window.run();

    return 0;
}

