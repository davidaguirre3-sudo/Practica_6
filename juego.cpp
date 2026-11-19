#include "juego.h"
#include "colisiones.h"
#include <cmath>

// --- constantes del juego (no estan en el diagrama simplificado, igual
// que las constantes de Actividad 1 no se mostraban como clase aparte) ---
const double DT = 0.02;                 // paso de tiempo de cada "tick" del timer
const int INTERVALO_TIMER_MS = 20;      // 1000*DT
const double UMBRAL_DETENCION = 0.05;   // rapidez por debajo de la cual el proyectil "se detuvo"
const double TIEMPO_MAXIMO_VUELO = 8.0; // segundos: red de seguridad
const int REBOTES_MAXIMOS = 25;         // red de seguridad
const double CONSTANTE_DANO = 1.0;      // danio = CONSTANTE_DANO * masa * rapidezImpacto

Juego::Juego(QObject* padre)
    : QObject(padre), caja(100, 60), turno(0), estado(ESTADO_ESPERANDO_DISPARO), e(0.7) {
    jugador1 = nullptr;
    jugador2 = nullptr;
    proyectil = nullptr;
    jugadorQueDisparo = nullptr;
    rebotes = 0;
    tiempoVuelo = 0.0;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, caja.getAncho(), caja.getAlto());

    // Fondo: una sola imagen que cubre toda la caja, detras de todo lo demas.
    auto* fondo = escena->addPixmap(
        QPixmap(":/sprites/fondo.png").scaled((int)caja.getAncho(), (int)caja.getAlto(),
                                              Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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
    delete escena;
    // 'timer' tiene a 'this' como padre QObject: en Qt real, Qt lo destruye
    // solo cuando se destruye 'this'. No se borra explicitamente aqui.
}

void Juego::iniciar() {
    turno = 0;
    estado = ESTADO_ESPERANDO_DISPARO;
    jugadorQueDisparo = nullptr;

    double ancho = caja.getAncho(), alto = caja.getAlto();
    jugador1 = new Jugador("Jugador 1", 10, alto - 5, 45);
    jugador2 = new Jugador("Jugador 2", ancho - 10, alto - 5, 135);

    // Las estructuras quedan a ras de piso (mismo nivel que los
    // representantes, que estan en y = alto-5) y pegadas a cada jugador,
    // como un muro que lo protege de los disparos del rival.
    double yPiso = alto - 5 - 8;  // el bloque de 8x8 queda con su base en el piso
    jugador1->agregarEstructura(new Estructura(Obstaculo(16, yPiso, 8, 8), 10));
    jugador1->agregarEstructura(new Estructura(Obstaculo(28, yPiso, 8, 8), 10));
    jugador2->agregarEstructura(new Estructura(Obstaculo(ancho - 24, yPiso, 8, 8), 10));
    jugador2->agregarEstructura(new Estructura(Obstaculo(ancho - 36, yPiso, 8, 8), 10));

    escena->addItem(jugador1->getRepresentante());
    escena->addItem(jugador2->getRepresentante());
    for (Estructura* est : jugador1->getInfraestructura()) escena->addItem(est);
    for (Estructura* est : jugador2->getInfraestructura()) escena->addItem(est);
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
    if (estado != ESTADO_ESPERANDO_DISPARO) return;  // ya hay un disparo en curso, o el juego termino

    Jugador* activo = getJugadorActivo();
    jugadorQueDisparo = activo;

    activo->getCanon()->setAngulo(angulo);
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

// Aplica la fisica de choques de la Actividad 1 (de Pedro) al proyectil en
// vuelo. Devuelve true si hubo un impacto directo contra un Representante
// (eso termina el turno de inmediato, sin importar la rapidez).
bool Juego::resolverColisionesProyectil() {
    Particula& cuerpo = proyectil->getCuerpo();

    if (choqueConPared(&cuerpo, caja)) {
        rebotes++;
    }

    Jugador* jugadores[2] = {jugador1, jugador2};
    for (Jugador* j : jugadores) {
        // Regla de "sin autodano": el jugador que disparo no daña su propia
        // infraestructura ni su propio Representante con su propio proyectil.
        bool esPropio = (j == jugadorQueDisparo);

        for (Estructura* est : j->getInfraestructura()) {
            if (est->estaDestruida()) continue;
            double rapidezAntes = proyectil->rapidezImpacto();
            if (choqueConObstaculo(&cuerpo, &est->getForma(), e)) {
                rebotes++;
                if (!esPropio) {
                    est->recibirDanio(CONSTANTE_DANO * cuerpo.getMasa() * rapidezAntes);
                }
            }
        }

        if (!esPropio) {
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

// Efecto visual: una explosion que aparece donde termino el disparo y se
// quita sola a los 300ms. No afecta la logica (daño/colisiones ya se
// resolvieron antes de llamar a esto).
void Juego::mostrarExplosion(double x, double y) {
    auto* fx = escena->addPixmap(
        QPixmap(":/sprites/explosion.png").scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    fx->setPos(x - 6, y - 6);
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