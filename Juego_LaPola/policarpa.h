#ifndef POLICARPA_H
#define POLICARPA_H

#include "personaje.h"

class Policarpa : public Personaje
{
    Q_OBJECT
public:
    Policarpa();

    void actualizar(double dt) override;

    // Acciones
    void moverIzquierda();
    void moverDerecha();
    void detener();
    void saltar();
    void agachar(bool valor);

    // Información para otras clases (guardias, Pacman)
    bool estaAgachada() const;
    double radioDeteccion() const;
    void setSueloY(double y);

private:
    bool agachada;
    bool enSuelo;
    double sueloY;      // altura del piso en la escena

    // Valores del personaje
    static constexpr double VELOCIDAD = 180.0;       // px/s caminando
    static constexpr double FACTOR_AGACHADA = 0.5;   // se mueve a la mitad
    static constexpr double GRAVEDAD = 1200.0;       // px/s²
    static constexpr double IMPULSO_SALTO = 650.0;   // px/s (ágil: salta alto)
    static constexpr double RADIO_NORMAL = 250.0;    // distancia a la que la ven
    static constexpr double RADIO_AGACHADA = 100.0;  // sigilosa: la ven de más cerca
};

#endif