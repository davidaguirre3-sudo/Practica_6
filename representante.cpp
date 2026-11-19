#include "representante.h"

const double Representante::RADIO = 3.0;

Representante::Representante(double x, double y, bool esJugador1) : vivo(true) {
    // La vista hace vista->scale(8,8) (ver VentanaPrincipal). Si aqui
    // escalamos el PNG directo a 14x20 pixeles, la vista despues lo estira
    // 8x y sale borroso/pixelado. En vez de eso: escalamos el pixmap a su
    // tamano final en pantalla (14*8 x 20*8) para conservar el detalle, y
    // compensamos con setScale(1/8) en el item para que en la escena siga
    // ocupando el mismo tamano (14x20 unidades) que antes.
    const double ESCALA_VISTA = 8.0;
    const double ANCHO = 14, ALTO = 20;
    QPixmap pix(esJugador1 ? ":/sprites/representante_azul.png"
                            : ":/sprites/representante_rojo.png");
    pix = pix.scaled((int)(ANCHO * ESCALA_VISTA), (int)(ALTO * ESCALA_VISTA),
                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(pix);
    setScale(1.0 / ESCALA_VISTA);
    // offset para que (x,y) quede en la BASE del personaje, no en su
    // esquina superior izquierda (en pixeles del pixmap ya escalado).
    setOffset(-pix.width() / 2.0, -pix.height());
    setPos(x, y);
}

void Representante::recibirImpacto() {
    vivo = false;
}

bool Representante::estaVivo() const {
    return vivo;
}
