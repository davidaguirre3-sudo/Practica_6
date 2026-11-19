#include "juego.h"
#include "colisiones.h"
#include <cmath>
#include <cstdlib>

const double DT = 0.02;
const int INTERVALO_TIMER_MS = 20;
const double UMBRAL_DETENCION = 0.05;
const double TIEMPO_MAXIMO_VUELO = 8.0;
const int REBOTES_MAXIMOS = 25;
const double CONSTANTE_DANO = 1.0;

Juego::Juego(QObject* padre)
    : QObject(padre), caja(150, 90), turno(0), estado(ESTADO_ESPERANDO_DISPARO), e(0.7) {
    jugador1 = nullptr;
    jugador2 = nullptr;
    proyectil = nullptr;
    jugadorQueDisparo = nullptr;
    rebotes = 0;
    tiempoVuelo = 0.0;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, caja.getAncho(), caja.getAlto());

    const double ESCALA_VISTA = 8.0;
    auto* fondo = escena->addPixmap(
        QPixmap(":/sprites/fondo.png").scaled((int)(caja.getAncho() * ESCALA_VISTA), (int)(caja.getAlto() * ESCALA_VISTA),
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    fondo->setScale(1.0 / ESCALA_VISTA);
    fondo->setZValue(-100);

    timer = new QTimer(this);
    timer->setInterval(INTERVALO_TIMER_MS);
    QObject::connect(timer, &QTimer::timeout, this, &Juego::actualizar);
}

Juego::~Juego() {
    timer->stop();
    if (proyectil) {
        escena->removeItem(proyectil);
        delete proyectil;
    }
    if (jugador1) {
        escena->removeItem(jugador1->getRepresentante());
        for (Estructura* est : jugador1->getInfraestructura()) escena->removeItem(est);
        delete jugador1;
    }
    if (jugador2) {
        escena->removeItem(jugador2->getRepresentante());
        for (Estructura* est : jugador2->getInfraestructura()) escena->removeItem(est);
        delete jugador2;
    }
    for (Estructura* obs : obstaculosNeutrales) {
        escena->removeItem(obs);
        delete obs;
    }
    delete escena;
}

void Juego::iniciar() {
    if (proyectil) {
        escena->removeItem(proyectil);
        delete proyectil;
        proyectil = nullptr;
    }
    if (jugador1) {
        escena->removeItem(jugador1->getRepresentante());
        for (Estructura* est : jugador1->getInfraestructura()) escena->removeItem(est);
        delete jugador1;
        jugador1 = nullptr;
    }
    if (jugador2) {
        escena->removeItem(jugador2->getRepresentante());
        for (Estructura* est : jugador2->getInfraestructura()) escena->removeItem(est);
        delete jugador2;
        jugador2 = nullptr;
    }
    for (Estructura* obs : obstaculosNeutrales) {
        escena->removeItem(obs);
        delete obs;
    }
    obstaculosNeutrales.clear();

    turno = 0;
    estado = ESTADO_ESPERANDO_DISPARO;
    jugadorQueDisparo = nullptr;

    double alto = caja.getAlto();
    const double DISTANCIA_ENTRE_JUGADORES = 120;
    const double margenLateral = (caja.getAncho() - DISTANCIA_ENTRE_JUGADORES) / 2.0;
    const double X_JUGADOR1 = margenLateral;
    const double X_JUGADOR2 = margenLateral + DISTANCIA_ENTRE_JUGADORES;
    jugador1 = new Jugador("Jugador 1", X_JUGADOR1, alto - 5, 45);
    jugador2 = new Jugador("Jugador 2", X_JUGADOR2, alto - 5, 135);

    const double ANCHO_REP = 14, ALTO_REP = 20;
    const double ANCHO_COLUMNA = 4, ALTO_TECHO = 4;
    const double RES_COLUMNA = 400, RES_TECHO = 150;

    auto crearBunker = [&](Jugador* j, double xBase, double yBase) {
        double repIzq = xBase - ANCHO_REP / 2;
        double repDer = xBase + ANCHO_REP / 2;
        double repArriba = (yBase - 5) - ALTO_REP;
        double altoColumna = alto - repArriba;

        j->agregarEstructura(new Estructura(
            Obstaculo(repIzq - ANCHO_COLUMNA, repArriba, ANCHO_COLUMNA, altoColumna), RES_COLUMNA));
        j->agregarEstructura(new Estructura(
            Obstaculo(repDer, repArriba, ANCHO_COLUMNA, altoColumna), RES_COLUMNA));
        j->agregarEstructura(new Estructura(
            Obstaculo(repIzq - ANCHO_COLUMNA, repArriba - ALTO_TECHO,
                      ANCHO_REP + 2 * ANCHO_COLUMNA, ALTO_TECHO), RES_TECHO));
    };

    crearBunker(jugador1, X_JUGADOR1, alto - 5);
    crearBunker(jugador2, X_JUGADOR2, alto - 5);

    const double ANCHO_OBSTACULO = 6, ALTO_OBSTACULO = 10, RES_OBSTACULO = 60;
    const double MARGEN_EXTREMOS = 0.12;
    const int NUM_OBSTACULOS = 4;
    const double anchoFranja = (1.0 - 2 * MARGEN_EXTREMOS) / NUM_OBSTACULOS;
    for (int i = 0; i < NUM_OBSTACULOS; i++) {
        double inicioFranja = MARGEN_EXTREMOS + anchoFranja * i;
        double azarX = static_cast<double>(std::rand()) / RAND_MAX;
        double fracX = inicioFranja + azarX * anchoFranja;
        double xCentro = X_JUGADOR1 + DISTANCIA_ENTRE_JUGADORES * fracX;

        double azarAltura = static_cast<double>(std::rand()) / RAND_MAX;
        double desdePiso = 35.0 + azarAltura * (80.0 - 35.0);
        double yArriba = alto - desdePiso;

        obstaculosNeutrales.push_back(new Estructura(
            Obstaculo(xCentro - ANCHO_OBSTACULO / 2, yArriba, ANCHO_OBSTACULO, ALTO_OBSTACULO),
            RES_OBSTACULO));
    }

    escena->addItem(jugador1->getRepresentante());
    escena->addItem(jugador2->getRepresentante());
    for (Estructura* est : jugador1->getInfraestructura()) escena->addItem(est);
    for (Estructura* est : jugador2->getInfraestructura()) escena->addItem(est);
    for (Estructura* obs : obstaculosNeutrales) escena->addItem(obs);
}

QGraphicsScene* Juego::getEscena() { return escena; }

Jugador* Juego::getJugadorActivo() { return turno == 0 ? jugador1 : jugador2; }

bool Juego::estaTerminado() const { return estado == ESTADO_TERMINADO; }

const Jugador* Juego::getGanador() const {
    if (jugador1->estaVencido()) return jugador2;
    if (jugador2->estaVencido()) return jugador1;
    return nullptr;
}

void Juego::disparar(double angulo, double velocidad) {
    if (estado != ESTADO_ESPERANDO_DISPARO) return;

    Jugador* activo = getJugadorActivo();
    jugadorQueDisparo = activo;

    double anguloEfectivo = (activo == jugador2) ? (180.0 - angulo) : angulo;

    activo->getCanon()->setAngulo(anguloEfectivo);
    activo->getCanon()->setVelocidad(velocidad);
    proyectil = activo->getCanon()->crearProyectil();
    escena->addItem(proyectil);

    rebotes = 0;
    tiempoVuelo = 0.0;
    estado = ESTADO_PROYECTIL_EN_VUELO;
    timer->start();
}

void Juego::actualizar() {
    if (estado != ESTADO_PROYECTIL_EN_VUELO || proyectil == nullptr) return;

    proyectil->avanzar(DT);
    tiempoVuelo += DT;

    bool impactoDirecto = resolverColisionesProyectil();

    bool detenido = proyectil->rapidezImpacto() < UMBRAL_DETENCION;
    bool seAgotoElTiempo = tiempoVuelo > TIEMPO_MAXIMO_VUELO;
    bool demasiadosRebotes = rebotes > REBOTES_MAXIMOS;

    if (impactoDirecto || detenido || seAgotoElTiempo || demasiadosRebotes) {
        terminarVueloProyectil();
    }
}

bool Juego::resolverColisionesProyectil() {
    Particula& cuerpo = proyectil->getCuerpo();

    if (choqueConPared(&cuerpo, caja)) {
        rebotes++;
    }

    for (Estructura* obs : obstaculosNeutrales) {
        if (obs->estaDestruida()) continue;
        double rapidezAntes = proyectil->rapidezImpacto();
        if (choqueConObstaculo(&cuerpo, &obs->getForma(), e)) {
            rebotes++;
            obs->recibirDanio(CONSTANTE_DANO * cuerpo.getMasa() * rapidezAntes);
        }
    }

    Jugador* jugadores[2] = {jugador1, jugador2};
    for (Jugador* j : jugadores) {
        bool esPropio = (j == jugadorQueDisparo);
        if (esPropio) continue;

        for (Estructura* est : j->getInfraestructura()) {
            if (est->estaDestruida()) continue;
            double rapidezAntes = proyectil->rapidezImpacto();
            if (choqueConObstaculo(&cuerpo, &est->getForma(), e)) {
                rebotes++;
                est->recibirDanio(CONSTANTE_DANO * cuerpo.getMasa() * rapidezAntes);
            }
        }

        Representante* rep = j->getRepresentante();
        if (rep->estaVivo()) {
            double dx = cuerpo.getX() - rep->pos().x();
            double dy = cuerpo.getY() - rep->pos().y();
            double distancia = std::sqrt(dx * dx + dy * dy);
            if (distancia < cuerpo.getRadio() + Representante::RADIO) {
                rep->recibirImpacto();
                return true;
            }
        }
    }
    return false;
}

void Juego::terminarVueloProyectil() {
    timer->stop();
    if (proyectil) {
        mostrarExplosion(proyectil->getCuerpo().getX(), proyectil->getCuerpo().getY());
        escena->removeItem(proyectil);
        delete proyectil;
        proyectil = nullptr;
    }
    if (verificarVictoria()) {
        estado = ESTADO_TERMINADO;
        emit juegoTerminado(getGanador()->getNombre());
    } else {
        cambiarTurno();
    }
}

void Juego::mostrarExplosion(double x, double y) {
    const double ESCALA_VISTA = 8.0;
    const double TAM = 12;
    auto* fx = escena->addPixmap(
        QPixmap(":/sprites/explosion.png").scaled((int)(TAM * ESCALA_VISTA), (int)(TAM * ESCALA_VISTA),
                                                    Qt::KeepAspectRatio, Qt::SmoothTransformation));
    fx->setScale(1.0 / ESCALA_VISTA);
    fx->setPos(x - TAM / 2, y - TAM / 2);
    fx->setZValue(50);
    QTimer::singleShot(300, this, [this, fx]() {
        escena->removeItem(fx);
        delete fx;
    });
}

bool Juego::verificarVictoria() {
    return jugador1->estaVencido() || jugador2->estaVencido();
}

void Juego::cambiarTurno() {
    turno = 1 - turno;
    estado = ESTADO_ESPERANDO_DISPARO;
    jugadorQueDisparo = nullptr;
    emit turnoCambiado(turno);
}
