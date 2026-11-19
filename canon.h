#ifndef CANON_H
#define CANON_H

#include "proyectil.h"

// STAND-IN temporal (parte de Pedro).
class Canon {
private:
    double x, y;
    double angulo;
    double velocidad;
    double masaProyectil;

public:
    Canon(double x, double y, double anguloInicial);

    void setAngulo(double angulo);
    void setVelocidad(double velocidad);
    Proyectil* crearProyectil();

    double getX() const;
    double getY() const;
};

#endif
