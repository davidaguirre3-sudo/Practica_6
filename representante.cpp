#include "representante.h"

const double Representante::RADIO = 3.0;

Representante::Representante(double x, double y, bool esJugador1) : vivo(true) {
    QPixmap pix(esJugador1 ? ":/sprites/representante_azul.png"
                           : ":/sprites/representante_rojo.png");
    pix = pix.scaled(14, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pix);
    // offset para que (x,y) quede en la BASE del personaje (como con el
    // circulo/rectangulo que usabamos antes de tener sprite), no en su
    // esquina superior izquierda.
    setOffset(-pix.width() / 2.0, -pix.height());
    setPos(x, y);
}

void Representante::recibirImpacto() {
    vivo = false;
}

bool Representante::estaVivo() const {
    return vivo;
}