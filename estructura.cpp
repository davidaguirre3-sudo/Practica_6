#include "estructura.h"
#include <QPen>
#include <QBrush>

Estructura::Estructura(const Obstaculo& f, double r) : forma(f), resistencia(r) {
    setRect(forma.getX(), forma.getY(), forma.getAncho(), forma.getAlto());
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::NoBrush));  // el rect sigue existiendo para la colision,
                                     // pero invisible: lo que se ve es el sprite

    sprite = new QGraphicsPixmapItem(this);  // hijo: se mueve/destruye con 'this'
    // La textura se elige sola segun la resistencia con la que la crean
    // (sin agregar parametros al constructor, para no romper el contrato
    // acordado con Pedro: Estructura(forma, resistencia) tal cual). Las
    // columnas del bunker (400, mas resistentes) usan la textura de
    // piedra; el techo (150, menos resistente) usa la de madera.
    const char* archivoSprite = (resistencia >= 300.0) ? ":/sprites/estructura.png"
                                                         : ":/sprites/caja_madera.png";

    // Deja un pequeño espacio VISUAL entre el sprite y el borde inferior:
    // solo se acorta lo que se DIBUJA, el rectangulo de colision (forma,
    // usado arriba en setRect) sigue llegando hasta el piso real, para que
    // no vuelva el hueco por donde se podia colar un disparo rasante.
    const double margenPiso = (forma.getAlto() * 0.3 < 6.0) ? (forma.getAlto() * 0.3) : 6.0;
    const double alturaVisible = forma.getAlto() - margenPiso;

    // mismo truco que en Representante: escalamos a tamano-de-pantalla y
    // compensamos con setScale(1/8) para que no quede pixelado al verse
    // ampliado por el vista->scale(8,8) de VentanaPrincipal.
    const double ESCALA_VISTA = 8.0;
    QPixmap pix(archivoSprite);
    pix = pix.scaled((int)(forma.getAncho() * ESCALA_VISTA), (int)(alturaVisible * ESCALA_VISTA),
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
