#include "obstaculo.h"

Obstaculo::Obstaculo(double x, double y, double ancho, double alto)
    : x(x), y(y), ancho(ancho), alto(alto)
{
}

double Obstaculo::getX() const
{
    return x;
}

double Obstaculo::getY() const
{
    return y;
}

double Obstaculo::getAncho() const
{
    return ancho;
}

double Obstaculo::getAlto() const
{
    return alto;
}