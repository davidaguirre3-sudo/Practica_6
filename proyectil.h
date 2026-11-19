#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include "particula.h"

// STAND-IN temporal (parte de Pedro). Cumple el contrato de
// CONTRATO_ACTIVIDAD2.md para que Juego/VentanaPrincipal puedan
// desarrollarse y probarse sin esperar su entrega.
class Proyectil : public QGraphicsEllipseItem {
private:
    Particula cuerpo;
    QGraphicsPixmapItem* sprite;  // agregado MIO para el dibujo

public:
    Proyectil(const Particula& p);

    void avanzar(double dt);
    double rapidezImpacto() const;
    Particula& getCuerpo();
};

#endif
