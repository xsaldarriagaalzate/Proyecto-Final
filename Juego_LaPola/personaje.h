#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"

class Personaje : public Entidad
{
    Q_OBJECT
public:
    Personaje(const QString &rutaSprite, int ancho, int alto,
              int vidas = 3, QGraphicsItem *padre = nullptr);
    ~Personaje() override = default;

    void actualizar(double dt) override;
    void setVelocidad(double vx, double vy);
    double velX() const;
    double velY() const;

    int getVidas() const;
    void perderVida();
    bool estaVivo() const;

protected:
    double vx;
    double vy;
    int vidas;
};

#endif // PERSONAJE_H