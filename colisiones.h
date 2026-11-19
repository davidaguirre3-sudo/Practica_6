#ifndef COLISIONES_H
#define COLISIONES_H

#include "particula.h"
#include "caja.h"
#include "obstaculo.h"

// Libreria con la fisica de los 3 tipos de choque.
// Cada funcion retorna true si hubo choque (para guardarlo en el archivo).

// Lados del obstaculo
const short LADO_NINGUNO   = 0;
const short LADO_IZQUIERDO = 1;
const short LADO_DERECHO   = 2;
const short LADO_ABAJO     = 3;
const short LADO_ARRIBA    = 4;

// 1. Particula contra pared: perfectamente elastica
bool choqueConPared(Particula* p, Caja caja);

// 2. Particula contra obstaculo: inelastica con coeficiente e
short ladoDeImpacto(Particula* p, Obstaculo* o);
bool choqueConObstaculo(Particula* p, Obstaculo* o, double e);

// 3. Particula contra particula: completamente inelastica (se fusionan)
bool choqueEntreParticulas(Particula* a, Particula* b);

#endif