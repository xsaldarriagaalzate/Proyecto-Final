#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "entidad.h"

class Enemigo : public Entidad
{
    Q_OBJECT
public:
    Enemigo(const QString &rutaSprite, int ancho, int alto,
            double velocidad, int vida = 1, QGraphicsItem *padre = nullptr);
    ~Enemigo() override = default;

    void recibirGolpe();
    bool estaVivo() const;
    double velocidad() const;

protected:
    double m_velocidad;
    int m_vida;
};

#endif // ENEMIGO_H