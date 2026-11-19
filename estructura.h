#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "obstaculo.h"

// STAND-IN temporal (parte de Pedro).
class Estructura : public QGraphicsRectItem {
private:
    Obstaculo forma;
    double resistencia;
    QGraphicsPixmapItem* sprite;  // agregado MIO para el dibujo; la logica
                                   // de colision sigue usando el rect normal

public:
    Estructura(const Obstaculo& forma, double resistencia);

    void recibirDanio(double danio);
    bool estaDestruida() const;
    Obstaculo& getForma();
};

#endif
