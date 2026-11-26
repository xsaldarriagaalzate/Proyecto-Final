#include "enemigo.h"

Enemigo::Enemigo(const QString &rutaSprite, int ancho, int alto,
                 double velocidad, int vida, QGraphicsItem *padre)
    : Entidad(rutaSprite, ancho, alto, padre),
    m_velocidad(velocidad),
    m_vida(vida)
{
}

void Enemigo::recibirGolpe()
{
    if (m_vida > 0)
        m_vida--;
}

bool Enemigo::estaVivo() const { return m_vida > 0; }

double Enemigo::velocidad() const { return m_velocidad; }