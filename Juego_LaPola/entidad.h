#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QString>

#include <QVector>
#include <QPixmap>

class Entidad : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Entidad(const QString &rutaSprite, int ancho, int alto,
            QGraphicsItem *padre = nullptr);
    ~Entidad() override = default;

    virtual void actualizar(double dt) = 0;

    bool colisionaCon(const Entidad *otra) const;

protected:
    static QVector<QPixmap> cargarAnimacion(const QString &ruta, int nCuadros, double alturaObjetivo);
};

#endif // ENTIDAD_H