#include <QApplication>
#include <cstdlib>
#include <chrono>
#include "ventanaprincipal.h"

int main(int argc, char *argv[]) {
    std::srand(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    QApplication a(argc, argv);
    VentanaPrincipal w;
    w.show();
    return a.exec();
}
