#include "estructura.h"
#include <QPen>
#include <QBrush>

Estructura::Estructura(const Obstaculo& f, double r) : forma(f), resistencia(r) {
    setRect(forma.getX(), forma.getY(), forma.getAncho(), forma.getAlto());
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::NoBrush));  // el rect sigue existiendo para la colision,
    // pero invisible: lo que se ve es el sprite

    sprite = new QGraphicsPixmapItem(this);

    const char* archivoSprite = (resistencia >= 300.0) ? ":/sprites/estructura.png"
                                                       : ":/sprites/caja_madera.png";

    const double ESCALA_VISTA = 8.0;
    QPixmap pix(archivoSprite);
    pix = pix.scaled((int)(forma.getAncho() * ESCALA_VISTA), (int)(forma.getAlto() * ESCALA_VISTA),
                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    sprite->setPixmap(pix);
    sprite->setScale(1.0 / ESCALA_VISTA);
    sprite->setPos(forma.getX(), forma.getY());
}

void Estructura::recibirDanio(double danio) {
    resistencia -= danio;
    if (estaDestruida()) sprite->setPixmap(QPixmap());  // la deja "vacia" al romperse
}

bool Estructura::estaDestruida() const {
    return resistencia <= 0.0;
}

Obstaculo& Estructura::getForma() {
    return forma;
}
