#include "nivel.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QDebug>

Nivel::Nivel(int numero, QObject *padre)
    : QObject(padre),
    m_numero(numero),
    escena(new QGraphicsScene(this)),
    m_pola(nullptr),
    tiempoRestanteSeg(0),
    metaX(0),
    fin(false),
    ultimaPlataforma(-1)
{
    connect(&reloj, &QTimer::timeout, this, &Nivel::tick);

    switch (m_numero)
    {
    case 1:
        construirNivel1();
        break;
    default:
        qDebug() << "El nivel" << m_numero << "todavía no existe.";
        break;
    }
}

// CONSTRUCCIÓN

void Nivel::construirNivel1()
{
    tiempoRestanteSeg = TIEMPO_NIVEL1;

    QPixmap hojaFondos(":/imagenes/fondos_nivel-1.png");

    if (hojaFondos.isNull())
    {

        qDebug() << "Error: no se encontró el fondo.";
        escena->setSceneRect(0, 0, 2400, 600);
        escena->setBackgroundBrush(Qt::black);
        metaX = 2400 - MARGEN_META;
    }
    else
    {
        int anchoOrig = hojaFondos.width();
        int altoFranja = hojaFondos.height() / 3;
        int xActual = 0;

        for (int i = 0; i < 3; ++i)
        {
            QPixmap franja = hojaFondos.copy(0, (i * altoFranja) + 4, anchoOrig, altoFranja - 8);
            QPixmap franjaEscalada = franja.scaledToHeight(600, Qt::FastTransformation);

            QGraphicsPixmapItem *itemFondo = new QGraphicsPixmapItem(franjaEscalada);
            itemFondo->setPos(xActual, 0);
            itemFondo->setZValue(-1);
            escena->addItem(itemFondo);

            xActual += franjaEscalada.width();
        }

        escena->setSceneRect(0, 0, xActual, 600);
        metaX = xActual - MARGEN_META;
    }

    m_pola = new Policarpa();
    m_pola->setSueloActivo(false);
    agregar(m_pola);

    // PLATAFORMAS  (PROVISIONALES: tejados de 500 px con huecos de 130 px)
    // Cuando las alinees con el fondo, reemplaza este bucle por llamadas a
    // agregarPlataforma(x, ySuperficie, ancho) con las medidas reales.
    const double alturas[] = {450, 410, 470};
    double anchoMapa = escena->sceneRect().width();
    int i = 0;

    for (double x = 0; x < anchoMapa; x += 630, ++i)
        agregarPlataforma(x, alturas[i % 3], 500);

    ultimaPlataforma = 0;
    puntoSeguro = QPointF(100, plataformas.first().top());
    reaparecer();
}

void Nivel::agregarPlataforma(double x, double ySuperficie, double ancho)
{

    plataformas.append(QRectF(x, ySuperficie, ancho, GROSOR_PLATAFORMA));

    if (MOSTRAR_PLATAFORMAS)
    {
        QGraphicsRectItem *guia = new QGraphicsRectItem(plataformas.last());
        guia->setBrush(QColor(0, 255, 0, 110));
        guia->setPen(Qt::NoPen);
        guia->setZValue(5);          // encima del fondo, debajo de la Pola
        escena->addItem(guia);
    }
}

// ACCESO

QGraphicsScene *Nivel::obtenerEscena() const { return escena; }
Policarpa *Nivel::pola() const { return m_pola; }
int Nivel::numero() const { return m_numero; }
double Nivel::tiempoRestante() const { return tiempoRestanteSeg; }
bool Nivel::terminado() const { return fin; }

// CONTROL DEL NIVEL

void Nivel::iniciar()
{
    if (!m_pola)
        return;     // el nivel no se construyó

    cronometro.start();
    reloj.start(16);
}

void Nivel::agregar(Entidad *e)
{
    escena->addItem(e);
    entidades.append(e);
}

void Nivel::penalizar(double segundos)
{
    tiempoRestanteSeg = qMax(0.0, tiempoRestanteSeg - segundos);
}

void Nivel::terminar(bool gano)
{
    if (fin)
        return;

    fin = true;
    reloj.stop();

    if (gano)
        emit victoria();
    else
        emit derrota();
}

void Nivel::tick()
{

    double dt = cronometro.restart() / 1000.0;

    if (dt > 0.05)
        dt = 0.05;

    double piesAntes = m_pola->pies();

    for (Entidad *e : entidades)
        e->actualizar(dt);

    resolverPlataformas(piesAntes);

    reglas(dt);

    emit actualizado();
}

// PLATAFORMAS

void Nivel::resolverPlataformas(double piesAntes)
{
    double piesAhora = m_pola->pies();

    if (piesAhora > CALLE_Y)
    {
        penalizar(CASTIGO_CAIDA);
        reaparecer();
        return;
    }

    int aterrizada = -1;

    if (m_pola->velY() >= 0)
    {
        double ancho = m_pola->boundingRect().width();
        double izq = m_pola->x() + ancho * MARGEN_PIES;
        double der = m_pola->x() + ancho * (1.0 - MARGEN_PIES);

        for (int i = 0; i < plataformas.size(); ++i)
        {
            const QRectF &p = plataformas[i];
            double techo = p.top();

            bool cruzo = piesAntes <= techo + TOLERANCIA && piesAhora >= techo;
            bool encima = der > p.left() && izq < p.right();

            if (cruzo && encima && (aterrizada < 0 || techo < plataformas[aterrizada].top()))
                aterrizada = i;
        }
    }

    if (aterrizada >= 0)
    {
        const QRectF &p = plataformas[aterrizada];
        m_pola->aterrizar(p.top());

        if (aterrizada != ultimaPlataforma)
        {
            ultimaPlataforma = aterrizada;
            puntoSeguro = QPointF(p.center().x() - m_pola->boundingRect().width() / 2.0,
                                  p.top());
        }
    }
    else
    {
        m_pola->setEnSuelo(false);
    }
}

void Nivel::reaparecer()
{
    m_pola->setX(puntoSeguro.x());
    m_pola->aterrizar(puntoSeguro.y());
    m_pola->setVelocidad(0, 0);
}

// REGLAS

void Nivel::reglas(double dt)
{
    switch (m_numero)
    {
    case 1:
        reglasNivel1(dt);
        break;
    }
}

void Nivel::reglasNivel1(double dt)
{
    tiempoRestanteSeg -= dt;

    if (m_pola->x() + m_pola->boundingRect().width() >= metaX)
    {
        terminar(true);
        return;
    }

    if (!m_pola->estaVivo())
    {
        terminar(false);
        return;
    }

    if (tiempoRestanteSeg <= 0)
    {
        tiempoRestanteSeg = 0;
        terminar(false);
    }
}