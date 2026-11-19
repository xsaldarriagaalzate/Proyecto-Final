#include "policarpa.h"

Policarpa::Policarpa()
    : Personaje(":/imagenes/pola_correr.png", 40, 60, 3),agachada(false),enSuelo(false),sueloY(600)
{
    QPixmap tira(":/imagenes/pola_correr.png");

    if (tira.isNull()) {
        qDebug() << "ERROR: No se encontró pola_correr.png";
        return;
    }

    int anchoCuadro = tira.width() / 8;

    if (anchoCuadro <= 0) {
        qDebug() << "ERROR: Tamaño incorrecto de la tira";
        return;
    }

    QPixmap cuadro = tira.copy(0, 0,anchoCuadro,tira.height());

    setPixmap(cuadro.scaledToHeight(250,Qt::FastTransformation));

    qDebug() << "Policarpa cargada correctamente";
    qDebug() << "Tamaño de la imagen:" << pixmap().size();
}

void Policarpa::actualizar(double dt)
{
    vy += GRAVEDAD * dt;
    Personaje::actualizar(dt);

    double piso = sueloY - pixmap().height();
    if (y() >= piso) {
        setY(piso);
        vy = 0;
        enSuelo = true;
    } else {
        enSuelo = false;
    }
}

void Policarpa::moverIzquierda()
{
    double v = agachada ? VELOCIDAD * FACTOR_AGACHADA : VELOCIDAD;
    vx = -v;
}

void Policarpa::moverDerecha()
{
    double v = agachada ? VELOCIDAD * FACTOR_AGACHADA : VELOCIDAD;
    vx = v;
}

void Policarpa::detener()
{
    vx = 0;
}

void Policarpa::saltar()
{
    if (enSuelo && !agachada) {
        vy = -IMPULSO_SALTO;
        enSuelo = false;
    }
}

void Policarpa::agachar(bool valor)
{
    agachada = valor;
    if (agachada) vx *= FACTOR_AGACHADA;
}

bool Policarpa::estaAgachada() const { return agachada; }

double Policarpa::radioDeteccion() const
{
    return agachada ? RADIO_AGACHADA : RADIO_NORMAL;
}

void Policarpa::setSueloY(double y) { sueloY = y; }