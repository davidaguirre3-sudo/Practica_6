#ifndef REPRESENTANTE_H
#define REPRESENTANTE_H

#include <QGraphicsPixmapItem>

class Representante : public QGraphicsPixmapItem {
private:
    bool vivo;

public:
    static const double RADIO;  // radio de colision fijo (impacto directo)

    Representante(double x, double y, bool esJugador1 = true);

    void recibirImpacto();
    bool estaVivo() const;
};

#endif
