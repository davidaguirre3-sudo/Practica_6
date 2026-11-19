#include "proyectil.h"
#include <QPen>
#include <QBrush>
#include <cmath>

Proyectil::Proyectil(const Particula& p) : cuerpo(p) {
    double r = cuerpo.getRadio();
    setRect(cuerpo.getX() - r, cuerpo.getY() - r, 2 * r, 2 * r);
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::NoBrush));  // el circulo sigue ahi para la colision, invisible

    sprite = new QGraphicsPixmapItem(this);
    const double ESCALA_VISTA = 8.0;  // igual razon que en Representante/Estructura
    QPixmap pix(":/sprites/proyectil.png");
    pix = pix.scaled((int)(2 * r * ESCALA_VISTA), (int)(2 * r * ESCALA_VISTA),
                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprite->setPixmap(pix);
    sprite->setScale(1.0 / ESCALA_VISTA);
    sprite->setPos(cuerpo.getX() - r, cuerpo.getY() - r);
}

void Proyectil::avanzar(double dt) {
    cuerpo.mover(dt);
    double r = cuerpo.getRadio();
    setRect(cuerpo.getX() - r, cuerpo.getY() - r, 2 * r, 2 * r);
    sprite->setPos(cuerpo.getX() - r, cuerpo.getY() - r);  // el sprite sigue al cuerpo
}

double Proyectil::rapidezImpacto() const {
    return std::sqrt(cuerpo.getVx() * cuerpo.getVx() + cuerpo.getVy() * cuerpo.getVy());
}

Particula& Proyectil::getCuerpo() {
    return cuerpo;
}
