
#include "personaje.h"

Personaje::Personaje(const QString &rutaSprite, int ancho, int alto,
                     int vidas, QGraphicsItem *padre)
    : QGraphicsPixmapItem(padre),
    vx(0),
    vy(0),
    vidas(vidas)
{
    QPixmap sprite(rutaSprite);

    if (sprite.isNull())
    {
        sprite = QPixmap(ancho, alto);
        sprite.fill(Qt::darkRed);
    }

    setPixmap(sprite);
    setZValue(10);
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
    if (vidas > 0)
        vidas--;
}

bool Personaje::estaVivo() const
{
    return vidas > 0;
}
