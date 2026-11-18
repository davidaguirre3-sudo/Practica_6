#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QGraphicsView>
#include "juego.h"

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

private:
    QGraphicsView* vista;
    Juego* juego;

public:
    VentanaPrincipal(QWidget* padre = nullptr);
    ~VentanaPrincipal();

public slots:
    void onDisparar(double angulo, double velocidad);

private slots:
    void onTurnoCambiado(int indiceJugadorActivo);
    void onJuegoTerminado(const std::string& nombreGanador);
};

#endif
