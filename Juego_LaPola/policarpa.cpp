#include "policarpa.h"
#include <QTransform>
#include <QtMath>
#include <QDebug>
#include <QGraphicsScene>

Policarpa::Policarpa()
    : Personaje(":/imagenes/pola_quieta.png", 64, 64, 3),
    agachada(false),
    enSuelo(false),
    sueloY(550),
    sueloActivo(true),
    direccion(0),
    animActual(nullptr),
    cuadroActual(0),
    tiempoCuadro(0),
    mirandoIzquierda(false),
    animRepite(true)
{

    animCorrer = cargarTira(":/imagenes/pola_correr.png", 10, ALTURA_DE_PIE);
    animSaltar = cargarTira(":/imagenes/pola_saltar.png", 8, ALTURA_DE_PIE);
    animQuieta = cargarTira(":/imagenes/pola_quieta.png", 8, ALTURA_DE_PIE);
    animAtacar = cargarTira(":/imagenes/pola_ataque.png", 8, ALTURA_DE_PIE);
    animDanio = cargarTira(":/imagenes/pola_danio.png", 8, ALTURA_DE_PIE);

    animActual = &animQuieta;

    if (!animQuieta.isEmpty())
        setPixmap(animQuieta[0]);

    efectoAtaque = new QGraphicsRectItem(this);
    efectoAtaque->setBrush(QColor(255, 60, 60, 120));
    efectoAtaque->setPen(Qt::NoPen);
    efectoAtaque->setVisible(false);
}

void Policarpa::actualizar(double dt)
{
    tiempoAtaque = qMax(0.0, tiempoAtaque - dt);
    esperaAtaque = qMax(0.0, esperaAtaque - dt);
    tiempoInvulnerable = qMax(0.0, tiempoInvulnerable - dt);

    double v = agachada ? VELOCIDAD * FACTOR_AGACHADA : VELOCIDAD;
    vx = direccion * v;

    vy += GRAVEDAD * dt;
    Personaje::actualizar(dt);

    if (sueloActivo)
    {
        double alto = pixmap().height();

        if (y() + alto >= sueloY)
        {
            setY(sueloY - alto);
            vy = 0;
            enSuelo = true;
        }
        else
        {
            enSuelo = false;
        }
    }

    if (scene())
    {
        double maxX = scene()->sceneRect().width() - pixmap().width();
        setX(qBound(0.0, x(), maxX));
    }

    actualizarAnimacion(dt);

    if (tiempoInvulnerable > 0.0)
        setOpacity((int(tiempoInvulnerable * 10) % 2) ? 0.35 : 1.0);
    else
        setOpacity(1.0);


    if (tiempoAtaque > 0.0)
    {
        efectoAtaque->setRect(zonaAtaqueLocal());
        efectoAtaque->setVisible(true);
    }
    else
    {
        efectoAtaque->setVisible(false);
    }
}

QRect Policarpa::limitesVisibles(const QImage &original)
{
    QImage imagen = original.convertToFormat(QImage::Format_ARGB32);

    int minX = imagen.width();
    int minY = imagen.height();
    int maxX = -1;
    int maxY = -1;

    for (int y = 0; y < imagen.height(); ++y)
    {
        for (int x = 0; x < imagen.width(); ++x)
        {
            if (qAlpha(imagen.pixel(x, y)) > 10)
            {
                minX = qMin(minX, x);
                minY = qMin(minY, y);
                maxX = qMax(maxX, x);
                maxY = qMax(maxY, y);
            }
        }
    }

    if (maxX < minX || maxY < minY)
        return QRect();

    return QRect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}


QVector<QPixmap> Policarpa::cargarTira(const QString &ruta, int nCuadros, double alturaObjetivo,
                                       double escalaFija, double *escalaUsada)
{
    QVector<QPixmap> cuadros;

    QPixmap tira(ruta);

    if (tira.isNull() || nCuadros <= 0)
    {
        qDebug() << "Error cargando animación:" << ruta;
        return cuadros;
    }


    QVector<QPixmap> recortes;
    int altoMaximo = 0;

    for (int i = 0; i < nCuadros; ++i)
    {
        int xInicio = (i * tira.width()) / nCuadros;
        int xFinal = ((i + 1) * tira.width()) / nCuadros;

        QPixmap frame = tira.copy(xInicio, 0, xFinal - xInicio, tira.height());

        QRect limites = limitesVisibles(frame.toImage());

        if (limites.isEmpty())
            continue;

        QPixmap recorte = frame.copy(limites);
        recortes.append(recorte);
        altoMaximo = qMax(altoMaximo, recorte.height());
    }

    if (altoMaximo == 0)
        return cuadros;

    double escala = (escalaFija > 0.0) ? escalaFija : alturaObjetivo / altoMaximo;

    if (escalaUsada)
        *escalaUsada = escala;

    for (const QPixmap &recorte : recortes)
    {
        cuadros.append(recorte.scaled(qRound(recorte.width() * escala),
                                      qRound(recorte.height() * escala),
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation));
    }

    return cuadros;
}


void Policarpa::actualizarAnimacion(double dt)
{
    const QVector<QPixmap> *nueva = nullptr;

    if (!enSuelo)
        nueva = &animSaltar;
    else if (qAbs(vx) > 0.01)
        nueva = &animCorrer;
    else
        nueva = &animQuieta;


    if (nueva->isEmpty())
        nueva = &animQuieta;

    if (nueva->isEmpty())
        return;


    if (nueva != animActual)
    {
        animActual = nueva;
        cuadroActual = 0;
        tiempoCuadro = 0;

        animRepite = (animActual != &animSaltar);
    }


    if (vx < -0.01)
        mirandoIzquierda = true;
    else if (vx > 0.01)
        mirandoIzquierda = false;


    tiempoCuadro += dt;

    while (tiempoCuadro >= DURACION_CUADRO)
    {
        tiempoCuadro -= DURACION_CUADRO;

        if (animRepite)
        {
            cuadroActual = (cuadroActual + 1) % animActual->size();
        }
        else if (cuadroActual < animActual->size() - 1)
        {
            cuadroActual++;
        }
        else
        {
            tiempoCuadro = 0;
            break;
        }
    }


    QPixmap cuadro = (*animActual)[cuadroActual];


    if (mirandoIzquierda)
        cuadro = cuadro.transformed(QTransform().scale(-1, 1));


    double pies = y() + pixmap().height();
    setPixmap(cuadro);
    setY(pies - cuadro.height());
}

void Policarpa::moverIzquierda() { direccion = -1; }
void Policarpa::moverDerecha()   { direccion = 1; }
void Policarpa::detener()        { direccion = 0; }

void Policarpa::saltar()
{
    if (enSuelo && !agachada)
    {
        vy = -IMPULSO_SALTO;
        enSuelo = false;
    }
}



void Policarpa::setSueloY(double y)
{
    sueloY = y;
}

double Policarpa::pies() const
{
    return y() + pixmap().height();
}

void Policarpa::aterrizar(double yPies)
{
    setY(yPies - pixmap().height());
    vy = 0;
    enSuelo = true;
}

void Policarpa::setEnSuelo(bool valor)
{
    enSuelo = valor;
}

void Policarpa::setSueloActivo(bool valor)
{
    sueloActivo = valor;
}

// COMBATE

void Policarpa::atacar()
{
    // No golpea si todavía está en la pausa o si está agachada
    if (esperaAtaque > 0.0 || agachada)
        return;

    tiempoAtaque = DURACION_ATAQUE;
    esperaAtaque = ESPERA_ATAQUE;
}


QRectF Policarpa::zonaAtaqueLocal() const
{
    double ancho = pixmap().width();
    double alto = pixmap().height();

    double x = mirandoIzquierda ? -ALCANCE_ATAQUE : ancho;
    return QRectF(x, alto * 0.2, ALCANCE_ATAQUE, alto * 0.6);
}

QRectF Policarpa::zonaAtaque() const
{
    if (tiempoAtaque <= 0.0)
        return QRectF();

    return mapRectToScene(zonaAtaqueLocal());
}

// Devuelve true si realmente perdió una vida
bool Policarpa::recibirDanio()
{
    if (tiempoInvulnerable > 0.0 || !estaVivo())
        return false;

    perderVida();
    tiempoInvulnerable = DURACION_INVULNERABLE;

    vy = -350.0;
    enSuelo = false;

    return true;
}