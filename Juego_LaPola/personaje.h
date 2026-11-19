#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Personaje(const QString &rutaSprite, int ancho, int alto,
              int vidas = 3, QGraphicsItem *padre = nullptr);
    virtual ~Personaje() = default;

    // Movimiento
    virtual void actualizar(double dt);
    void setVelocidad(double vx, double vy);
    double velX() const;
    double velY() const;

    // Vidas
    int getVidas() const;
    void perderVida();
    bool estaVivo() const;

protected:
    double vx;      // velocidad horizontal (píxeles por segundo)
    double vy;      // velocidad vertical (píxeles por segundo)
    int vidas;
};

#endif // PERSONAJE_H