#include "entidad.h"
#include <QPixmap>

Entidad::Entidad(const QString &rutaSprite, int ancho, int alto, QGraphicsItem *padre)
    : QGraphicsPixmapItem(padre)
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

bool Entidad::colisionaCon(const Entidad *otra) const
{
    if (!otra)
        return false;

    // Compara los rectángulos de ambas entidades (rápido y predecible)
    return collidesWithItem(otra, Qt::IntersectsItemBoundingRect);
}