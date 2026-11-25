#include "nivel.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QDebug>
#include <QImage>
#include <QCursor>

static QVector<QPixmap> cortarEnBandas(const QPixmap &hoja)
{
    QVector<QPixmap> piezas;
    QImage imagen = hoja.toImage().convertToFormat(QImage::Format_ARGB32);

    int inicio = -1;
    int minX = 0;
    int maxX = 0;

    for (int y = 0; y <= imagen.height(); ++y)
    {
        int filaMin = imagen.width();
        int filaMax = -1;

        if (y < imagen.height())
        {
            for (int x = 0; x < imagen.width(); ++x)
            {
                if (qAlpha(imagen.pixel(x, y)) > 10)
                {
                    filaMin = qMin(filaMin, x);
                    filaMax = qMax(filaMax, x);
                }
            }
        }

        bool filaConDibujo = (filaMax >= 0);

        if (filaConDibujo)
        {
            if (inicio < 0)
            {
                inicio = y;
                minX = filaMin;
                maxX = filaMax;
            }
            else
            {
                minX = qMin(minX, filaMin);
                maxX = qMax(maxX, filaMax);
            }
        }
        else if (inicio >= 0)
        {
            piezas.append(hoja.copy(minX, inicio, maxX - minX + 1, y - inicio));
            inicio = -1;
        }
    }

    return piezas;
}

static QPixmap recortarTransparencia(const QPixmap &original)
{
    QImage imagen = original.toImage().convertToFormat(QImage::Format_ARGB32);

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
        return original;

    return original.copy(minX, minY, maxX - minX + 1, maxY - minY + 1);
}

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

    cargarSprites();

    agregarTecho(-2, 462, 280);
    agregarTecho(346, 508, 280);
    agregarTecho(585, 547, 280);
    agregarTecho(885, 481, 280);
    agregarTecho(1421, 495, 280);
    agregarTecho(2009, 461, 280);
    agregarTecho(2343, 487, 280);
    agregarTecho(2696, 538, 280);
    agregarTecho(3228, 463, 280);
    agregarTecho(3891, 517, 280);
    agregarTecho(4163, 475, 280);
    agregarTecho(4483, 482, 280);
    agregarTecho(5069, 496, 280);
    agregarTecho(5342, 530, 280);
    agregarTecho(5860, 484, 280);
    agregarTecho(6290, 478, 280);
    agregarTecho(6722, 459, 280);
    agregarTecho(7032, 511, 280);
    agregarTecho(7624, 501, 280);
    agregarTecho(7980, 453, 280);
    agregarTecho(8276, 506, 280);
    agregarTecho(8861, 470, 280);
    agregarTecho(9124, 538, 280);
    agregarTecho(9618, 493, 280);
    agregarTecho(9944, 532, 280);
    agregarTecho(10455, 460, 280);
    agregarTecho(10979, 469, 280);
    agregarTecho(11283, 452, 280);
    agregarTecho(11710, 518, 280);
    agregarTecho(11977, 480, 280);
    agregarTecho(12781, 426, 280);
    agregarTecho(13057, 502, 280);
    agregarTecho(13479, 509, 280);
    agregarTecho(13729, 449, 280);
    agregarTecho(14306, 519, 280);
    agregarTecho(14740, 458, 280);
    agregarTecho(15111, 455, 280);
    agregarTecho(15476, 519, 280);
    agregarTecho(15949, 485, 280);
    agregarTecho(16228, 525, 280);
    agregarTecho(16800, 450, 280);
    agregarLadrillo(275, 357, 0);
    agregarLadrillo(581, 368, 1);
    agregarLadrillo(905, 264, 2);
    agregarLadrillo(1219, 454, 0);
    agregarLadrillo(1715, 360, 1);
    agregarLadrillo(2347, 330, 2);
    agregarLadrillo(2489, 170, 0);
    agregarLadrillo(2966, 372, 1);
    agregarLadrillo(3551, 388, 2);
    agregarLadrillo(3605, 195, 0);
    agregarLadrillo(4029, 305, 1);
    agregarLadrillo(4745, 362, 2);
    agregarLadrillo(5325, 351, 0);
    agregarLadrillo(5642, 378, 1);
    agregarLadrillo(6080, 313, 2);
    agregarLadrillo(6565, 304, 0);
    agregarLadrillo(7010, 330, 1);
    agregarLadrillo(7319, 425, 2);
    agregarLadrillo(7850, 330, 0);
    agregarLadrillo(8197, 292, 1);
    agregarLadrillo(8530, 334, 2);
    agregarLadrillo(9134, 232, 0);
    agregarLadrillo(9344, 372, 1);
    agregarLadrillo(9875, 258, 2);
    agregarLadrillo(10246, 405, 0);
    agregarLadrillo(10761, 330, 1);
    agregarLadrillo(11124, 201, 2);
    agregarLadrillo(11518, 324, 0);
    agregarLadrillo(12050, 330, 1);
    agregarLadrillo(12367, 460, 2);
    agregarLadrillo(12569, 231, 0);
    agregarLadrillo(13285, 321, 1);
    agregarLadrillo(13680, 284, 2);
    agregarLadrillo(14134, 382, 0);
    agregarLadrillo(14463, 354, 1);
    agregarLadrillo(14802, 231, 2);
    agregarLadrillo(15410, 330, 0);
    agregarLadrillo(15730, 366, 1);
    agregarLadrillo(16203, 330, 2);
    agregarLadrillo(16592, 262, 0);

    ultimaPlataforma = 0;
    puntoSeguro = QPointF(100, rectanguloPiso(plataformas.first()).top());
    reaparecer();
}

void Nivel::cargarSprites()
{
    spriteTecho = recortarTransparencia(QPixmap(":/imagenes/techo.png"));
    spritesLadrillo = cortarEnBandas(QPixmap(":/imagenes/plataformas.png"));

    qDebug() << "techo.png cargado:" << !spriteTecho.isNull()
             << "| piezas encontradas en plataformas.png:" << spritesLadrillo.size()
             << "(deberían ser 3)";

    if (spriteTecho.isNull())
    {
        spriteTecho = QPixmap(90, 90);
        spriteTecho.fill(QColor(160, 70, 50));
    }

    while (spritesLadrillo.size() < 3)
    {
        QPixmap relleno(60 * (spritesLadrillo.size() + 1), 25);
        relleno.fill(QColor(120, 120, 120));
        spritesLadrillo.append(relleno);
    }
}

void Nivel::agregarTecho(double x, double ySuperficie, double ancho)
{
    QPixmap dibujo = spriteTecho.scaledToWidth(qRound(ancho), Qt::FastTransformation);
    agregarPlataforma(dibujo, x, ySuperficie, MARGEN_TECHO, DESFASE_TECHO, -1);
}

void Nivel::agregarLadrillo(double x, double ySuperficie, int tipo)
{
    const QPixmap &original = spritesLadrillo[tipo];

    QPixmap dibujo = original.scaled(qRound(original.width() * ESCALA_LADRILLO),
                                     qRound(original.height() * ESCALA_LADRILLO),
                                     Qt::IgnoreAspectRatio, Qt::FastTransformation);

    agregarPlataforma(dibujo, x, ySuperficie, MARGEN_LADRILLO, DESFASE_LADRILLO, tipo);
}

void Nivel::agregarPlataforma(const QPixmap &dibujo, double x, double ySuperficie, double margenLateral, double desfase, int tipo)
{
    double ancho = dibujo.width();

    QGraphicsPixmapItem *imagen = new QGraphicsPixmapItem(dibujo);
    imagen->setPos(x, ySuperficie - desfase);
    imagen->setZValue(4);

    imagen->setData(DATO_TIPO, tipo);
    imagen->setData(DATO_MARGEN, margenLateral);
    imagen->setData(DATO_DESFASE, desfase);

    if (MODO_EDICION)
    {
        imagen->setFlag(QGraphicsItem::ItemIsMovable, true);
        imagen->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
        imagen->setCursor(Qt::OpenHandCursor);
    }

    escena->addItem(imagen);
    plataformas.append(imagen);

    // Para depurar: el piso en verde. Es HIJO del dibujo, así que se mueve con él
    if (MOSTRAR_PLATAFORMAS)
    {
        QRectF local(ancho * margenLateral, desfase,
                     ancho * (1.0 - 2.0 * margenLateral), GROSOR_PLATAFORMA);

        QGraphicsRectItem *guia = new QGraphicsRectItem(local, imagen);
        guia->setBrush(QColor(0, 255, 0, 110));
        guia->setPen(Qt::NoPen);
        guia->setAcceptedMouseButtons(Qt::NoButton);   // los clics pasan al dibujo
    }
}

// El rectángulo donde SE PISA se calcula con la posición ACTUAL del dibujo
QRectF Nivel::rectanguloPiso(const QGraphicsPixmapItem *plataforma) const
{
    double ancho = plataforma->pixmap().width();
    double margen = plataforma->data(DATO_MARGEN).toDouble();
    double desfase = plataforma->data(DATO_DESFASE).toDouble();

    return QRectF(plataforma->x() + ancho * margen,
                  plataforma->y() + desfase,
                  ancho * (1.0 - 2.0 * margen),
                  GROSOR_PLATAFORMA);
}

// MODO EDICIÓN: imprime las plataformas tal como las dejaste
void Nivel::imprimirPlataformas() const
{
    qDebug().noquote() << "---- copia estas líneas en construirNivel1() ----";

    for (const QGraphicsPixmapItem *p : plataformas)
    {
        int x = qRound(p->x());
        int y = qRound(p->y() + p->data(DATO_DESFASE).toDouble());   // altura de la superficie
        int tipo = p->data(DATO_TIPO).toInt();

        if (tipo < 0)
            qDebug().noquote() << QString("    agregarTecho(%1, %2, %3);")
                                      .arg(x).arg(y).arg(p->pixmap().width());
        else
            qDebug().noquote() << QString("    agregarLadrillo(%1, %2, %3);")
                                      .arg(x).arg(y).arg(tipo);
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
    QRectF pisoAterrizada;

    if (m_pola->velY() >= 0)
    {
        double ancho = m_pola->boundingRect().width();
        double izq = m_pola->x() + ancho * MARGEN_PIES;
        double der = m_pola->x() + ancho * (1.0 - MARGEN_PIES);

        for (int i = 0; i < plataformas.size(); ++i)
        {
            QRectF p = rectanguloPiso(plataformas[i]);
            double techo = p.top();

            bool cruzo = piesAntes <= techo + TOLERANCIA && piesAhora >= techo;
            bool encima = der > p.left() && izq < p.right();

            if (cruzo && encima && (aterrizada < 0 || techo < pisoAterrizada.top()))
            {
                aterrizada = i;
                pisoAterrizada = p;
            }
        }
    }

    if (aterrizada >= 0)
    {
        m_pola->aterrizar(pisoAterrizada.top());

        if (aterrizada != ultimaPlataforma)
        {
            ultimaPlataforma = aterrizada;
            puntoSeguro = QPointF(pisoAterrizada.center().x() - m_pola->boundingRect().width() / 2.0,
                                  pisoAterrizada.top());
        }
    }
    else
    {
        m_pola->setEnSuelo(false);   // está en el aire (saltando o cayendo)
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
    if (!MODO_EDICION)
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