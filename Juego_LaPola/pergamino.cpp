#include "pergamino.h"
#include <QDebug>

Pergamino::Pergamino()
    : Entidad(":/imagenes/mensaje.png", 32, 32),
    cuadroActual(0),
    tiempoCuadro(0)
{
    QPixmap hoja(":/imagenes/mensaje.png");

    if (hoja.isNull())
    {
        qDebug() << "No se encontró la hoja del mensaje (revisa el .qrc)";
        return;
    }

    int anchoCuadro = hoja.width() / NUM_CUADROS;

    for (int i = 0; i < NUM_CUADROS; ++i)
    {
        QPixmap cuadro = hoja.copy(i * anchoCuadro, 0, anchoCuadro, hoja.height());
        cuadros.append(cuadro.scaledToHeight(ALTO_EN_PANTALLA, Qt::FastTransformation));
    }

    setPixmap(cuadros[0]);
}

void Pergamino::actualizar(double dt)
{
    if (cuadros.isEmpty())
        return;

    tiempoCuadro += dt;

    if (tiempoCuadro >= DURACION_CUADRO)
    {
        tiempoCuadro -= DURACION_CUADRO;
        cuadroActual = (cuadroActual + 1) % cuadros.size();
        setPixmap(cuadros[cuadroActual]);
    }
}