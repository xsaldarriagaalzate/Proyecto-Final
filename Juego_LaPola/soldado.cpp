#include "soldado.h"

#include <QPixmap>
#include <QColor>
#include <QLineF>
#include <QTransform>

Soldado::Soldado(const QVector<QPointF> &ruta)
    : Enemigo(":/imagenes/guardia_quieto.png", ANCHO, ALTO, VELOCIDAD, 1),
    rutaPatrulla(ruta),
    destino(1)
{

    animCaminar = cargarAnimacion(":/imagenes/guardia_caminando.png", 8, ALTO);

    if (!animCaminar.isEmpty())
    {
        setPixmap(animCaminar[0]);
    }
    else
    {

        QPixmap relleno(ANCHO, ALTO);
        relleno.fill(QColor(40, 70, 130));
        setPixmap(relleno);
    }

    if (!rutaPatrulla.isEmpty())
    {
        actual = rutaPatrulla.first();
        setPos(actual.x(), actual.y() - pixmap().height());
    }
}

void Soldado::actualizar(double dt)
{
    if (!estaVivo() || rutaPatrulla.size() < 2)
        return;

    QPointF objetivo = rutaPatrulla[destino];
    QPointF falta = objetivo - actual;
    double distancia = QLineF(actual, objetivo).length();
    double paso = m_velocidad * dt;

    if (falta.x() < -0.01)
        mirandoIzquierda = true;
    else if (falta.x() > 0.01)
        mirandoIzquierda = false;

    if (distancia <= paso)
    {
        actual = objetivo;
        destino = (destino + 1) % rutaPatrulla.size();
    }
    else
    {
        actual += falta * (paso / distancia);
    }

    actualizarAnimacion(dt);

    setPos(actual.x(), actual.y() - pixmap().height());
}

void Soldado::actualizarAnimacion(double dt)
{
    if (animCaminar.isEmpty())
        return;

    tiempoCuadro += dt;

    while (tiempoCuadro >= DURACION_CUADRO)
    {
        tiempoCuadro -= DURACION_CUADRO;
        cuadroActual = (cuadroActual + 1) % animCaminar.size();
    }

    QPixmap cuadro = animCaminar[cuadroActual];

    if (mirandoIzquierda == SPRITE_MIRA_DERECHA)
        cuadro = cuadro.transformed(QTransform().scale(-1, 1));

    setPixmap(cuadro);
}