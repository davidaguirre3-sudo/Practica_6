#ifndef CAJA_H
#define CAJA_H

// Caja rectangular donde se mueven las particulas.
// Va desde la esquina (0, 0) hasta la esquina (ancho, alto).
class Caja {
private:
    double ancho;
    double alto;

public:
    Caja(double ancho, double alto);

    double getAncho() const;
    double getAlto() const;
};

#endif