#include "personaje.h"

Personaje::Personaje(const QString &rutaSprite, int ancho, int alto,
                     int vidas, QGraphicsItem *padre)
    : QGraphicsPixmapItem(padre), vx(0), vy(0), vidas(vidas)
{
    QPixmap sprite(rutaSprite);
    if (sprite.isNull()) {              // si aún no existe el sprite,
        sprite = QPixmap(ancho, alto);  // se dibuja un rectángulo de color
        sprite.fill(Qt::darkRed);
    }
    setPixmap(sprite.scaled(ancho, alto));
}

void Personaje::actualizar(double dt)
{
    setPos(x() + vx * dt, y() + vy * dt);
}

void Personaje::setVelocidad(double nuevaVx, double nuevaVy)
{
    vx = nuevaVx;
    vy = nuevaVy;
}

double Personaje::velX() const { return vx; }
double Personaje::velY() const { return vy; }

int Personaje::getVidas() const { return vidas; }

void Personaje::perderVida()
{
    if (vidas > 0) vidas--;
}

bool Personaje::estaVivo() const { return vidas > 0; }