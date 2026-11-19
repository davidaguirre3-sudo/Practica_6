#include "estructura.h"
#include <QPen>
#include <QBrush>

Estructura::Estructura(const Obstaculo& f, double r) : forma(f), resistencia(r) {
    setRect(forma.getX(), forma.getY(), forma.getAncho(), forma.getAlto());
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::NoBrush));  // el rect sigue existiendo para la colision,
    // pero invisible: lo que se ve es el sprite

    sprite = new QGraphicsPixmapItem(this);  // hijo: se mueve/destruye con 'this'
    // alterno entre las dos texturas que generaron, solo para que no todas
    // las estructuras se vean identicas.
    static int contador = 0;
    const char* archivo = (contador++ % 2 == 0) ? ":/sprites/estructura.png"
                                                : ":/sprites/caja_madera.png";
    QPixmap pix(archivo);
    sprite->setPixmap(pix.scaled((int)forma.getAncho(), (int)forma.getAlto()));
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