#include "canon.h"
#include <cmath>

static int siguienteIdProyectil = 0;

const double RADIO_PROYECTIL = 1.0;

Canon::Canon(double x_, double y_, double anguloInicial)
    : x(x_), y(y_), angulo(anguloInicial), velocidad(0.0), masaProyectil(1.0) {}

void Canon::setAngulo(double a) { angulo = a; }
void Canon::setVelocidad(double v) { velocidad = v; }

Proyectil* Canon::crearProyectil() {
    double rad = angulo * M_PI / 180.0;
    double vx = velocidad * std::cos(rad);
    double vy = -velocidad * std::sin(rad);  // y crece hacia abajo en la escena
    Particula p(siguienteIdProyectil++, x, y, vx, vy, masaProyectil, RADIO_PROYECTIL);
    return new Proyectil(p);
}

double Canon::getX() const { return x; }
double Canon::getY() const { return y; }
