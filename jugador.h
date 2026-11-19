#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <vector>
#include "canon.h"
#include "representante.h"
#include "estructura.h"

// STAND-IN temporal (parte de Pedro).
class Jugador {
private:
    std::string nombre;
    std::vector<Estructura*> infraestructura;
    Representante* representante;
    Canon* canon;

public:
    Jugador(const std::string& nombre, double xBase, double yBase, double anguloInicial);
    ~Jugador();

    bool estaVencido() const;

    const std::string& getNombre() const;
    Canon* getCanon();
    Representante* getRepresentante();
    std::vector<Estructura*>& getInfraestructura();
    void agregarEstructura(Estructura* e);
};

#endif
