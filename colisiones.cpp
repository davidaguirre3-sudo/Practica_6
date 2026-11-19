#include "colisiones.h"
#include <cmath>

// =====================================================================
// 1. CHOQUE CON PARED (perfectamente elastico, e = 1)
//    Se invierte la velocidad PERPENDICULAR a la pared.
//    La paralela no cambia. La rapidez se conserva -> no se pierde energia.
//      Pared izquierda o derecha:  vx' = -vx   (vy igual)
//      Pared de abajo o arriba:    vy' = -vy   (vx igual)
// =====================================================================
bool choqueConPared(Particula* p, Caja caja)
{
    double r = p->getRadio();
    bool choco = false;

    // Pared izquierda (x = 0)
    if (p->getX() - r < 0) {
        p->setX(r);                          // la saca de la pared
        p->setVx(-p->getVx());
        choco = true;
    }
    // Pared derecha (x = ancho)
    else if (p->getX() + r > caja.getAncho()) {
        p->setX(caja.getAncho() - r);
        p->setVx(-p->getVx());
        choco = true;
    }

    // Pared de abajo (y = 0)
    if (p->getY() - r < 0) {
        p->setY(r);
        p->setVy(-p->getVy());
        choco = true;
    }
    // Pared de arriba (y = alto)
    else if (p->getY() + r > caja.getAlto()) {
        p->setY(caja.getAlto() - r);
        p->setVy(-p->getVy());
        choco = true;
    }

    return choco;
}

// =====================================================================
// 2. CHOQUE CON OBSTACULO (inelastico, coeficiente de restitucion e)
// =====================================================================

// Con que lado del obstaculo choco la particula.
// Se mide cuanto se metio la particula por cada lado:
// el lado por donde MENOS se metio es por donde entro.
// (Simplificacion: la particula se toma como un cuadrado de lado 2*radio)
short ladoDeImpacto(Particula* p, Obstaculo* o)
{
    double r = p->getRadio();

    double porIzquierda = (p->getX() + r) - o->getX();
    double porDerecha   = (o->getX() + o->getAncho()) - (p->getX() - r);
    double porAbajo     = (p->getY() + r) - o->getY();
    double porArriba    = (o->getY() + o->getAlto()) - (p->getY() - r);

    // Si por algun lado no se toca, no hay choque
    if (porIzquierda <= 0 || porDerecha <= 0 || porAbajo <= 0 || porArriba <= 0)
        return LADO_NINGUNO;

    // Buscar el menor
    double menor = porIzquierda;
    short lado = LADO_IZQUIERDO;

    if (porDerecha < menor) { menor = porDerecha; lado = LADO_DERECHO; }
    if (porAbajo   < menor) { menor = porAbajo;   lado = LADO_ABAJO;   }
    if (porArriba  < menor) { menor = porArriba;  lado = LADO_ARRIBA;  }

    return lado;
}

// Rebote con perdida de energia:
//   v_perpendicular' = -e * v_perpendicular
//   v_paralela'      =  v_paralela
//   Lado izquierdo o derecho: la perpendicular es vx
//   Lado de abajo o arriba:   la perpendicular es vy
bool choqueConObstaculo(Particula* p, Obstaculo* o, double e)
{
    short lado = ladoDeImpacto(p, o);
    if (lado == LADO_NINGUNO)
        return false;

    double r = p->getRadio();

    if (lado == LADO_IZQUIERDO) {
        p->setX(o->getX() - r);                          // la saca del obstaculo
        p->setVx(-e * std::fabs(p->getVx()));            // sale hacia la izquierda
    }
    else if (lado == LADO_DERECHO) {
        p->setX(o->getX() + o->getAncho() + r);
        p->setVx(e * std::fabs(p->getVx()));             // sale hacia la derecha
    }
    else if (lado == LADO_ABAJO) {
        p->setY(o->getY() - r);
        p->setVy(-e * std::fabs(p->getVy()));            // sale hacia abajo
    }
    else if (lado == LADO_ARRIBA) {
        p->setY(o->getY() + o->getAlto() + r);
        p->setVy(e * std::fabs(p->getVy()));             // sale hacia arriba
    }
    return true;
}

// =====================================================================
// 3. CHOQUE ENTRE PARTICULAS (completamente inelastico: se pegan)
//    Conservacion del momento lineal:
//      m1*v1 + m2*v2 = (m1 + m2) * v'
//      v' = (m1*v1 + m2*v2) / (m1 + m2)
//    El nuevo cuerpo:
//      masa     M = m1 + m2
//      posicion = centro de masa
//      radio    = sqrt(r1^2 + r2^2)   (el area es la suma de las dos areas)
//    La particula "a" se queda con todo y la "b" se desactiva.
// =====================================================================
bool choqueEntreParticulas(Particula* a, Particula* b)
{
    if (!a->estaActiva() || !b->estaActiva())
        return false;

    // Chocan si la distancia entre centros es menor que la suma de radios
    double dx = a->getX() - b->getX();
    double dy = a->getY() - b->getY();
    double distancia = std::sqrt(dx * dx + dy * dy);

    if (distancia >= a->getRadio() + b->getRadio())
        return false;

    double m1 = a->getMasa();
    double m2 = b->getMasa();
    double M = m1 + m2;

    // Nueva velocidad (conservacion del momento)
    double vx = (m1 * a->getVx() + m2 * b->getVx()) / M;
    double vy = (m1 * a->getVy() + m2 * b->getVy()) / M;

    // Nueva posicion (centro de masa)
    double x = (m1 * a->getX() + m2 * b->getX()) / M;
    double y = (m1 * a->getY() + m2 * b->getY()) / M;

    // Nuevo radio (se conserva el area)
    double r = std::sqrt(a->getRadio() * a->getRadio() + b->getRadio() * b->getRadio());

    a->setVx(vx);
    a->setVy(vy);
    a->setX(x);
    a->setY(y);
    a->setMasa(M);
    a->setRadio(r);
    b->desactivar();

    return true;
}