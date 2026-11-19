#ifndef PARTICULA_H
#define PARTICULA_H

// Particula circular. Guarda sus datos y sabe moverse.
// La fisica de los choques esta en colisiones.h
class Particula {
private:
    int id;
    double x, y;      // posicion del centro
    double vx, vy;    // velocidad
    double masa;
    double radio;
    bool activa;      // false cuando fue absorbida en una fusion

public:
    Particula(int id, double x, double y, double vx, double vy, double masa, double radio);

    // Getters
    int getId() const { return id; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getVx() const { return vx; }
    double getVy() const { return vy; }
    double getMasa() const { return masa; }
    double getRadio() const { return radio; }
    bool estaActiva() const { return activa; }

    // Setters (los usan las funciones de colisiones)
    void setX(double nx) { x = nx; }
    void setY(double ny) { y = ny; }
    void setVx(double nvx) { vx = nvx; }
    void setVy(double nvy) { vy = nvy; }
    void setMasa(double m) { masa = m; }
    void setRadio(double r) { radio = r; }
    void desactivar() { activa = false; }

    // Ecuacion de movimiento en un paso de tiempo dt
    void mover(double dt);
};

#endif
