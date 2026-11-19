#ifndef OBSTACULO_H
#define OBSTACULO_H

// Obstaculo cuadrado y quieto dentro de la caja.
// (x, y) es su esquina inferior izquierda.
class Obstaculo {
private:
    double x, y;
    double ancho, alto;

public:
    Obstaculo(double x, double y, double ancho, double alto);

    double getX() const;
    double getY() const;
    double getAncho() const;
    double getAlto() const;
};

#endif