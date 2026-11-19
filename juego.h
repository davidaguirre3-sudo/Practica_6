#ifndef JUEGO_H
#define JUEGO_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <vector>
#include "jugador.h"
#include "proyectil.h"
#include "estructura.h"
#include "caja.h"

const short ESTADO_ESPERANDO_DISPARO = 0;
const short ESTADO_PROYECTIL_EN_VUELO = 1;
const short ESTADO_TERMINADO = 2;

class Juego : public QObject {
    Q_OBJECT

private:
    QGraphicsScene* escena;
    QTimer* timer;
    Jugador* jugador1;
    Jugador* jugador2;
    Proyectil* proyectil;
    Caja caja;
    std::vector<Estructura*> obstaculosNeutrales;
    short turno;
    short estado;
    double e;

    Jugador* jugadorQueDisparo;
    int rebotes;
    double tiempoVuelo;

public:
    Juego(QObject* padre = nullptr);
    ~Juego();

    void iniciar();
    void disparar(double angulo, double velocidad);

    QGraphicsScene* getEscena();
    Jugador* getJugadorActivo();
    bool estaTerminado() const;
    const Jugador* getGanador() const;

signals:
    void turnoCambiado(int indiceJugadorActivo);
    void juegoTerminado(const std::string& nombreGanador);

private slots:
    void actualizar();

private:
    void cambiarTurno();
    bool verificarVictoria();
    bool resolverColisionesProyectil();
    void terminarVueloProyectil();
    void mostrarExplosion(double x, double y);
};

#endif
