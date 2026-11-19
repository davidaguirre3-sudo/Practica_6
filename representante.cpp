#include "representante.h"

const double Representante::RADIO = 3.0;

Representante::Representante(double x, double y, bool esJugador1) : vivo(true) {
    const double ESCALA_VISTA = 8.0;
    const double ANCHO = 14, ALTO = 20;
    QPixmap pix(esJugador1 ? ":/sprites/representante_azul.png"
                           : ":/sprites/representante_rojo.png");
    pix = pix.scaled((int)(ANCHO * ESCALA_VISTA), (int)(ALTO * ESCALA_VISTA),
                     Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pix);
    setScale(1.0 / ESCALA_VISTA);
    setOffset(-pix.width() / 2.0, -pix.height());
    setPos(x, y);
}

void Representante::recibirImpacto() {
    vivo = false;
}

bool Representante::estaVivo() const {
    return vivo;
}