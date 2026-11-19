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

// Estados del juego (controlan que puede pasar en cada momento).
const short ESTADO_ESPERANDO_DISPARO = 0;  // el jugador activo puede disparar
const short ESTADO_PROYECTIL_EN_VUELO = 1; // hay que esperar a que se resuelva
const short ESTADO_TERMINADO = 2;          // ya hay un ganador

class Juego : public QObject {
    Q_OBJECT

private:
    QGraphicsScene* escena;
    QTimer* timer;
    Jugador* jugador1;
    Jugador* jugador2;
    Proyectil* proyectil;
    Caja caja;
    std::vector<Estructura*> obstaculosNeutrales;  // sueltos en el mapa, no
    // son de ningun jugador
    // (siempre chocan/dañan,
    // sin regla de autodano)
    short turno;   // 0 = le toca a jugador1, 1 = le toca a jugador2
    short estado;
    double e;      // coeficiente de restitucion contra obstaculos

    // --- bookkeeping interno, no forma parte del diagrama simplificado
    // (igual que las constantes/indices auxiliares de la Actividad 1) ---
    Jugador* jugadorQueDisparo;  // para la regla de "sin autodano"
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
    const Jugador* getGanador() const;  // valido solo si estaTerminado()

signals:
    void turnoCambiado(int indiceJugadorActivo);
    void juegoTerminado(const std::string& nombreGanador);

private slots:
    void actualizar();  // conectado a timer->timeout()

private:
    void cambiarTurno();
    bool verificarVictoria();
    bool resolverColisionesProyectil();  // true si hubo impacto directo a un Representante
    void terminarVueloProyectil();
    void mostrarExplosion(double x, double y);  // efecto visual, agregado mio
};

#endif
