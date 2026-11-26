#include "entidad.h"

#include <QPixmap>
#include <QImage>
#include <QDebug>

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

QVector<QPixmap> Entidad::cargarAnimacion(const QString &ruta, int nCuadros, double alturaObjetivo)
{
    QVector<QPixmap> cuadros;

    QPixmap tira(ruta);

    if (tira.isNull() || nCuadros <= 0)
    {
        qDebug() << "Error cargando animación:" << ruta;
        return cuadros;
    }

    QVector<QPixmap> recortes;
    int altoMaximo = 0;

    for (int i = 0; i < nCuadros; ++i)
    {
        int xInicio = (i * tira.width()) / nCuadros;
        int xFinal = ((i + 1) * tira.width()) / nCuadros;

        QPixmap frame = tira.copy(xInicio, 0, xFinal - xInicio, tira.height());
        QImage imagen = frame.toImage().convertToFormat(QImage::Format_ARGB32);

        int minX = imagen.width();
        int minY = imagen.height();
        int maxX = -1;
        int maxY = -1;

        for (int y = 0; y < imagen.height(); ++y)
        {
            for (int x = 0; x < imagen.width(); ++x)
            {
                if (qAlpha(imagen.pixel(x, y)) > 10)
                {
                    minX = qMin(minX, x);
                    minY = qMin(minY, y);
                    maxX = qMax(maxX, x);
                    maxY = qMax(maxY, y);
                }
            }
        }

        if (maxX < minX || maxY < minY)
            continue;       // cuadro vacío

        QPixmap recorte = frame.copy(minX, minY, maxX - minX + 1, maxY - minY + 1);
        recortes.append(recorte);
        altoMaximo = qMax(altoMaximo, recorte.height());
    }

    if (altoMaximo == 0)
        return cuadros;

    // PASADA 2: un solo factor para toda la tira
    double escala = alturaObjetivo / altoMaximo;

    for (int i = 0; i < recortes.size(); ++i)
    {
        cuadros.append(recortes[i].scaled(qRound(recortes[i].width() * escala),
                                          qRound(recortes[i].height() * escala),
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation));
    }

    qDebug() << ruta << "| cuadros:" << cuadros.size()
             << "| alto sin escalar:" << altoMaximo << "px | escala:" << escala;

    return cuadros;
}