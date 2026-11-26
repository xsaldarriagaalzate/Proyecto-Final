#ifndef PERGAMINO_H
#define PERGAMINO_H

#include <QVector>
#include <QPixmap>
#include "entidad.h"

class Pergamino : public Entidad
{
    Q_OBJECT
public:
    Pergamino();
    void actualizar(double dt) override;

private:
    QVector<QPixmap> cuadros;
    int cuadroActual;
    double tiempoCuadro;

    static constexpr int NUM_CUADROS = 8;
    static constexpr double ALTO_EN_PANTALLA = 40.0;
    static constexpr double DURACION_CUADRO = 0.12;
};

#endif