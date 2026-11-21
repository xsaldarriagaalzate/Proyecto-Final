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
    direccion(0),
    animActual(nullptr),
    cuadroActual(0),
    tiempoCuadro(0),
    mirandoIzquierda(false),
    animRepite(true)
{
    // Estas tiras traen lienzos de tamaños distintos, así que cada una se escala
    // para que la Pola mida la altura que queremos en pantalla.
    double escalaDePie = 0.0;

    animCorrer = cargarTira(":/imagenes/pola_correr.png", 8, ALTURA_DE_PIE);
    animSaltar = cargarTira(":/imagenes/pola_saltar.png", 8, ALTURA_DE_PIE);
    animQuieta = cargarTira(":/imagenes/pola_quieta.png", 2, ALTURA_DE_PIE, 0.0, &escalaDePie);

    // agachada-quieta tiene el MISMO lienzo que quieta (1774 x 887), o sea, fue dibujada
    // a la misma escala: se reutiliza el factor de la Pola de pie. Así la diferencia de
    // altura entre estar de pie y agachada es la real del dibujo, no un número inventado.
    animAgachadaQuieta = cargarTira(":/imagenes/pola_agachada-quieta.png", 2, ALTURA_AGACHADA,
                                    escalaDePie);

    // La tira de caminar agachada se ajusta a la altura que quedó la agachada quieta
    double altoAgachada = 0.0;
    for (const QPixmap &cuadro : animAgachadaQuieta)
        altoAgachada = qMax(altoAgachada, double(cuadro.height()));

    if (altoAgachada <= 0.0)
        altoAgachada = ALTURA_AGACHADA;   // respaldo si no cargó la imagen

    animAgachada = cargarTira(":/imagenes/pola_agachada.png", 8, altoAgachada);

    animActual = &animQuieta;

    if (!animQuieta.isEmpty())
        setPixmap(animQuieta[0]);
}

void Policarpa::actualizar(double dt)
{
    // 1. La velocidad horizontal se DERIVA del estado actual en cada frame.
    //    Así no importa en qué orden se presionen o suelten las teclas.
    double v = agachada ? VELOCIDAD * FACTOR_AGACHADA : VELOCIDAD;
    vx = direccion * v;

    // 2. Gravedad y movimiento
    vy += GRAVEDAD * dt;
    Personaje::actualizar(dt);

    // 3. Colisión con el piso, medida con los PIES (borde inferior del sprite),
    //    porque la altura del sprite cambia según la animación.
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

    // 4. Límites del mapa (no salirse por la izquierda ni por la derecha)
    if (scene())
    {
        double maxX = scene()->sceneRect().width() - pixmap().width();
        setX(qBound(0.0, x(), maxX));
    }

    actualizarAnimacion(dt);
}

// Devuelve el rectángulo que contiene los píxeles no transparentes
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

    // Cuadro vacío: devuelve un rectángulo vacío
    if (maxX < minX || maxY < minY)
        return QRect();

    return QRect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

// Carga una tira de sprites y deja a la Pola con la altura objetivo (en píxeles de pantalla)
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

    // PASADA 1: recortar cada cuadro al personaje y buscar el recorte más alto
    QVector<QPixmap> recortes;
    int altoMaximo = 0;

    for (int i = 0; i < nCuadros; ++i)
    {
        // Dividir la imagen horizontalmente
        int xInicio = (i * tira.width()) / nCuadros;
        int xFinal = ((i + 1) * tira.width()) / nCuadros;

        QPixmap frame = tira.copy(xInicio, 0, xFinal - xInicio, tira.height());

        // Detectar los límites del personaje
        QRect limites = limitesVisibles(frame.toImage());

        // Evitar cuadros vacíos
        if (limites.isEmpty())
            continue;

        QPixmap recorte = frame.copy(limites);
        recortes.append(recorte);
        altoMaximo = qMax(altoMaximo, recorte.height());
    }

    if (altoMaximo == 0)
        return cuadros;

    // PASADA 2: UN solo factor para toda la tira, así los cuadros conservan
    // sus proporciones entre sí y no "laten" de tamaño al animarse
    // Si nos dan una escala fija se usa tal cual; si no, se calcula para llegar a la altura objetivo
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

    qDebug() << ruta
             << "| tira:" << tira.size()
             << "| cuadros:" << cuadros.size()
             << "| Pola más alta sin escalar:" << altoMaximo << "px"
             << "| escala:" << escala;

    return cuadros;
}


void Policarpa::actualizarAnimacion(double dt)
{
    const QVector<QPixmap> *nueva = nullptr;

    // Seleccionar animación
    if (!enSuelo)
        nueva = &animSaltar;
    else if (agachada && qAbs(vx) > 0.01)
        nueva = &animAgachada;
    else if (agachada)
        nueva = &animAgachadaQuieta;
    else if (qAbs(vx) > 0.01)
        nueva = &animCorrer;
    else
        nueva = &animQuieta;

    // Comprobar que exista la animación
    if (nueva->isEmpty())
        nueva = &animQuieta;

    if (nueva->isEmpty())
        return;

    // Cambiar animación
    if (nueva != animActual)
    {
        animActual = nueva;
        cuadroActual = 0;
        tiempoCuadro = 0;

        // Solo el salto se reproduce una vez
        animRepite = (animActual != &animSaltar);
    }

    // Orientación
    if (vx < -0.01)
        mirandoIzquierda = true;
    else if (vx > 0.01)
        mirandoIzquierda = false;

    // Actualizar cuadros
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

    // Obtener cuadro actual
    QPixmap cuadro = (*animActual)[cuadroActual];

    // Voltear si mira a la izquierda
    if (mirandoIzquierda)
        cuadro = cuadro.transformed(QTransform().scale(-1, 1));

    // Como los cuadros tienen alturas distintas, hay que mantener los PIES en su
    // sitio: guardamos dónde estaban, cambiamos el dibujo y recolocamos la Y.
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

void Policarpa::agachar(bool valor)
{
    agachada = valor;   // ya no toca vx: se calcula en actualizar()
}

bool Policarpa::estaAgachada() const
{
    return agachada;
}

double Policarpa::radioDeteccion() const
{
    return agachada ? RADIO_AGACHADA : RADIO_NORMAL;
}

void Policarpa::setSueloY(double y)
{
    sueloY = y;
}