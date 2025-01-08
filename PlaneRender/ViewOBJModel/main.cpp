#include "MainWindow.h"
#include "KDTree.h"

#include "random"

int main() {
    //Sounds soundEngine();

    MainWindow& window = MainWindow::instance();

    Sound::playMusic(window.currentPath, Sound::ForestPath);

    window.run();

    return 0;
}

