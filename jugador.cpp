#include "jugador.h"

Jugador::Jugador(const std::string& nombre_, double xBase, double yBase, double anguloInicial)
    : nombre(nombre_) {
    // El canon dispara desde la altura del pecho del Representante (no
    // desde el piso): el Representante mide 20 de alto y su base (pies)
    // queda en yBase-5, asi que el pecho queda ~15 mas arriba que yBase.
    canon = new Canon(xBase, yBase - 15, anguloInicial);
    // El color del sprite lo deduzco del nombre ("Jugador 1"/"Jugador 2")
    // para no tener que agregarle un parametro a ESTE constructor, que si
    // es del contrato real que usa Juego.cpp.
    bool esJugador1 = nombre_.find('1') != std::string::npos;
    representante = new Representante(xBase, yBase - 5, esJugador1);
}

Jugador::~Jugador() {
    delete canon;
    delete representante;
    for (Estructura* e : infraestructura) delete e;
}

bool Jugador::estaVencido() const {
    return !representante->estaVivo();
}

const std::string& Jugador::getNombre() const { return nombre; }
Canon* Jugador::getCanon() { return canon; }
Representante* Jugador::getRepresentante() { return representante; }
std::vector<Estructura*>& Jugador::getInfraestructura() { return infraestructura; }

void Jugador::agregarEstructura(Estructura* e) {
    infraestructura.push_back(e);
}
