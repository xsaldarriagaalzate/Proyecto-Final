#ifndef SOLDADO_H
#define SOLDADO_H

#include "enemigo.h"

#include <QVector>
#include <QPointF>
#include <QPixmap>

// Soldado realista: patrulla entre los puntos de su ruta.
class Soldado : public Enemigo
{
    Q_OBJECT
public:
    static constexpr int ANCHO = 50;
    static constexpr int ALTO = 96;

    explicit Soldado(const QVector<QPointF> &ruta);

    void actualizar(double dt) override;

private:
    QVector<QPointF> rutaPatrulla;
    int destino;
    QPointF actual;

    void actualizarAnimacion(double dt);

    QVector<QPixmap> animCaminar;
    int cuadroActual = 0;
    double tiempoCuadro = 0.0;
    bool mirandoIzquierda = false;

    static constexpr double DURACION_CUADRO = 0.12;     // s por cuadro
    static constexpr bool SPRITE_MIRA_DERECHA = true;   // false si tu dibujo original mira a la izquierda

    static constexpr double VELOCIDAD = 70.0;   // px/s
};

#endif // SOLDADO_H