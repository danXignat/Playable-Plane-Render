#include "MainWindow.h"
#include "KDTree.h"
#include "Sounds.h"

#include "random"

int main() {
    Sounds soundEngine();

    MainWindow& window = MainWindow::instance();

    window.run();

    return 0;
}

