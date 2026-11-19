#ifndef REPRESENTANTE_H
#define REPRESENTANTE_H

#include <QGraphicsPixmapItem>

// STAND-IN temporal (parte de Pedro). El parametro 'esJugador1' es un
// agregado MIO solo para poder cargarle el sprite correcto (azul/rojo) en
// esta copia de prueba -- no es parte del contrato que le pasamos a Pedro,
// por eso tiene valor por defecto: con eso, si en algun momento mezclan
// esto con su Representante real (que seguira el contrato original de 2
// parametros), nada deja de compilar.
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
