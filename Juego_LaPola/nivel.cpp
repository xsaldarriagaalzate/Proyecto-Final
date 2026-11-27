#include "nivel.h"

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QDebug>
#include <QImage>
#include <QPen>

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
    fin(false),
    ultimaPlataforma(-1)
{
    connect(&reloj, &QTimer::timeout, this, &Nivel::tick);

    switch (m_numero)
    {
    case 1:
        construirNivel1();
        break;
    case 2:
        construirNivel2();
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
    }

    m_pola = new Policarpa();
    m_pola->setSueloActivo(false);
    agregar(m_pola);

    cargarSprites();

    agregarTecho(-2, 462, 280);      // 0
    agregarTecho(346, 508, 280);     // 1
    agregarTecho(585, 547, 280);     // 2
    agregarTecho(885, 481, 280);     // 3
    agregarTecho(1421, 495, 280);    // 4
    agregarTecho(2009, 461, 280);    // 5
    agregarTecho(2343, 487, 280);    // 6
    agregarTecho(2696, 538, 280);    // 7
    agregarTecho(3228, 463, 280);    // 8
    agregarTecho(3891, 517, 280);    // 9
    agregarTecho(4163, 475, 280);    // 10
    agregarTecho(4483, 482, 280);    // 11
    agregarTecho(5069, 496, 280);    // 12
    agregarTecho(5342, 530, 280);    // 13
    agregarTecho(5860, 484, 280);    // 14
    agregarTecho(6290, 478, 280);    // 15
    agregarTecho(6722, 459, 280);    // 16
    agregarTecho(7032, 511, 280);    // 17
    agregarTecho(7624, 501, 280);    // 18
    agregarTecho(7980, 453, 280);    // 19
    agregarTecho(8276, 506, 280);    // 20
    agregarTecho(8861, 470, 280);    // 21
    agregarTecho(9124, 538, 280);    // 22
    agregarTecho(9618, 493, 280);    // 23
    agregarTecho(9944, 532, 280);    // 24
    agregarTecho(10455, 460, 280);   // 25
    agregarTecho(10979, 469, 280);   // 26
    agregarTecho(11283, 452, 280);   // 27
    agregarTecho(11710, 518, 280);   // 28
    agregarTecho(11977, 480, 280);   // 29
    agregarTecho(12781, 426, 280);   // 30
    agregarTecho(13100, 420, 280);   // 31
    agregarTecho(13479, 509, 280);   // 32
    agregarTecho(13729, 449, 280);   // 33
    agregarTecho(14306, 519, 280);   // 34
    agregarTecho(14740, 458, 280);   // 35
    agregarTecho(15111, 455, 280);   // 36
    agregarTecho(15476, 519, 280);   // 37
    agregarTecho(15949, 485, 280);   // 38
    agregarTecho(16228, 525, 280);   // 39
    agregarTecho(16550, 450, 280);   // 40
    agregarLadrillo(275, 357, 0);    // 41
    agregarLadrillo(581, 368, 1);    // 42
    agregarLadrillo(905, 264, 2);    // 43
    agregarLadrillo(1219, 454, 0);   // 44
    agregarLadrillo(1715, 360, 1);   // 45
    agregarLadrillo(2347, 330, 2);   // 46
    agregarLadrillo(2489, 170, 0);   // 47
    agregarLadrillo(2966, 372, 1);   // 48
    agregarLadrillo(3551, 388, 2);   // 49
    agregarLadrillo(3605, 195, 0);   // 50
    agregarLadrillo(4029, 305, 1);   // 51
    agregarLadrillo(4745, 362, 2);   // 52
    agregarLadrillo(5325, 351, 0);   // 53
    agregarLadrillo(5642, 378, 1);   // 54
    agregarLadrillo(6080, 313, 2);   // 55
    agregarLadrillo(6565, 304, 0);   // 56
    agregarLadrillo(7010, 330, 1);   // 57
    agregarLadrillo(7319, 425, 2);   // 58
    agregarLadrillo(7850, 330, 0);   // 59
    agregarLadrillo(8197, 292, 1);   // 60
    agregarLadrillo(8530, 334, 2);   // 61
    agregarLadrillo(9134, 232, 0);   // 62
    agregarLadrillo(9344, 372, 1);   // 63
    agregarLadrillo(9875, 258, 2);   // 64
    agregarLadrillo(10246, 405, 0);  // 65
    agregarLadrillo(10761, 330, 1);  // 66
    agregarLadrillo(11124, 201, 2);  // 67
    agregarLadrillo(11518, 324, 0);  // 68
    agregarLadrillo(12050, 330, 1);  // 69
    agregarLadrillo(12367, 460, 2);  // 70
    agregarLadrillo(12569, 231, 0);  // 71
    agregarLadrillo(13285, 324, 1);  // 72
    agregarLadrillo(13680, 284, 2);  // 73
    agregarLadrillo(14134, 382, 0);  // 74
    agregarLadrillo(14380, 260, 1);  // 75
    agregarLadrillo(14662, 200, 2);  // 76
    agregarLadrillo(15410, 330, 0);  // 77
    agregarLadrillo(15630, 206, 1);  // 78
    agregarLadrillo(16203, 330, 2);  // 79
    agregarLadrillo(16592, 262, 0);  // 80

    agregarSoldado(4);
    agregarSoldado(11);
    agregarSoldado(15);
    agregarSoldado(19);
    agregarSoldado(23);
    agregarSoldado(28);
    agregarSoldado(32);
    agregarSoldado(37);
    agregarSoldado(41);
    agregarSoldado(45);
    agregarSoldado(48);
    agregarSoldado(53);
    agregarSoldado(58);
    agregarSoldado(65);
    agregarSoldado(67);
    agregarSoldado(71);
    agregarSoldado(76);
    agregarSoldado(79);

    agregarPergamino(3, 0.8);
    agregarPergamino(11, 0.7);
    agregarPergamino(18, 0.2);
    agregarPergamino(25, 0.5);
    agregarPergamino(34, 0.4);
    agregarPergamino(41, 0.3);
    agregarPergamino(48, 0.1);
    agregarPergamino(56, 0.5);
    agregarPergamino(67, 0.7);
    agregarPergamino(74, 0.8);

    agregarBuzon(40);
    buzon->setZValue(6);

    ultimaPlataforma = 0;
    puntoSeguro = QPointF(100, rectanguloPiso(plataformas.first()).top());
    reaparecer();
}

void Nivel::construirNivel2()
{
    QPixmap hoja(":/imagenes/fondos_nivel-2.png");
    const int N = 3;
    int tw = hoja.width()  / N;
    int th = hoja.height() / N;

    int mapa[N][N] = { {0, 1, 2},
                       {3, 4, 5},
                       {6, 7, 8} };

    for (int f = 0; f < N; ++f) {
        for (int c = 0; c < N; ++c) {
            int k = mapa[f][c];
            QPixmap baldosa = hoja.copy((k % N) * tw, (k / N) * th, tw, th);
            QGraphicsPixmapItem *it = escena->addPixmap(baldosa);
            it->setPos(c * tw, f * th);
            it->setZValue(0);
        }
    }
    int anchoMapa = N * tw, altoMapa = N * th;
    escena->setSceneRect(0, 0, anchoMapa, altoMapa);

    // auto agregarPared = [this](qreal x, qreal y, qreal w, qreal h) {
    //     QGraphicsRectItem *p = escena->addRect(0, 0, w, h, QPen(Qt::NoPen), QBrush(Qt::NoBrush));
    //     // para depurar: QBrush(QColor(255, 0, 0, 80)) en vez de NoBrush
    //     p->setPos(x, y);
    //     p->setData(0, "pared");
    //     p->setZValue(0.5);
    // };

    const bool DEPURAR_PAREDES = true;

    auto agregarPared = [this, DEPURAR_PAREDES](qreal x, qreal y, qreal w, qreal h) {
        QBrush pincel = DEPURAR_PAREDES ? QBrush(QColor(255, 0, 0, 90))
                                        : QBrush(Qt::NoBrush);
        QGraphicsRectItem *p = escena->addRect(0, 0, w, h, QPen(Qt::NoPen), pincel);
        p->setPos(x, y);
        p->setData(0, "pared");
        p->setZValue(0.5);
    };

    // Bordes del mapa
    agregarPared(-20, 0, 20, altoMapa);              // izquierda
    agregarPared(anchoMapa, 0, 20, altoMapa);        // derecha
    agregarPared(0, -20, anchoMapa, 20);             // arriba
    agregarPared(0, altoMapa, anchoMapa, 20);        // abajo

    // Obstáculos internos (x, y, ancho, alto)

    agregarPared(145, 208, 95, 95);
    agregarPared(70, 192, 15, 150);
    agregarPared(330, 15, 135, 50);
    agregarPared(306, 192, 15, 150);
    agregarPared(15, 15, 320, 30);
    agregarPared(435, 410, 20, 20);

    agregarPared(468, 255, 20, 180);
    agregarPared(755, 255, 20, 180);
    agregarPared(468, 15, 20, 120);
    agregarPared(755, 15, 20, 120);
    agregarPared(505, 415, 245, 20);

    agregarPared(775, 255, 65, 20);
    agregarPared(775, 75, 100, 20);
    agregarPared(880, 15, 270, 25);
    agregarPared(1160, 15, 20, 190);
    agregarPared(820, 275, 20, 60);
    agregarPared(900, 280, 180, 5);
    agregarPared(845, 360, 290, 5);
    agregarPared(1095, 115, 70, 50);

    agregarPared(410, 410, 20, 90);
    agregarPared(15, 435, 320, 30);
    agregarPared(340, 465, 20, 30);
    agregarPared(15, 630, 20, 100);
    agregarPared(30, 755, 55, 20);
    agregarPared(310, 755, 55, 20);
    agregarPared(85, 795, 55, 20);
    agregarPared(240, 795, 55, 20);
    agregarPared(370, 620, 20, 110);

    agregarPared(445, 490, 65, 20);
    agregarPared(505, 530, 65, 20);
    agregarPared(570, 540, 245, 20);
    agregarPared(390, 620, 70, 20);
    agregarPared(460, 630, 20, 180);
    agregarPared(540, 675, 20, 135);
    agregarPared(565, 675, 245, 20);

    agregarPared(820, 520, 350, 20);
    agregarPared(1160, 370, 20, 280);
    agregarPared(1090, 800, 150, 20);
    agregarPared(890, 800, 100, 20);

    agregarPared(40, 830, 100, 20);
    agregarPared(20, 850, 20, 100);
    agregarPared(240, 830, 100, 20);
    agregarPared(340, 850, 20, 70);
    agregarPared(370, 900, 50, 20);
    agregarPared(20, 1000, 50, 20);
    agregarPared(70, 1020, 35, 20);
    agregarPared(105, 1040, 35, 20);
    agregarPared(140, 1060, 15, 90);
    agregarPared(315, 1000, 100, 20);
    agregarPared(285, 1020, 35, 20);
    agregarPared(245, 1040, 35, 20);
    agregarPared(230, 1060, 15, 90);
    agregarPared(155, 1150, 70, 20);

    agregarPared(430, 800, 20, 100);
    agregarPared(430, 1000, 20, 210);
    agregarPared(460, 1210, 70, 20);
    agregarPared(710, 1210, 70, 20);
    agregarPared(540, 850, 190, 40);
    agregarPared(730, 750, 20, 220);
    agregarPared(820, 820, 20, 200);
    agregarPared(750, 1090, 20, 100);
    agregarPared(770, 1120, 50, 20);

    agregarPared(835, 1020, 20, 10);
    agregarPared(855, 1040, 20, 10);
    agregarPared(875, 1060, 120, 10);
    agregarPared(835, 1150, 20, 10);
    agregarPared(855, 1180, 240, 10);
    agregarPared(1100, 1060, 20, 120);
    agregarPared(930, 880, 20, 140);
    agregarPared(980, 830, 20, 110);
    agregarPared(1080, 830, 20, 110);
    agregarPared(1110, 940, 140, 20);
    agregarPared(1110, 1060, 140, 20);


    polaCen = new PolaCenital();
    polaCen->setPos(anchoMapa / 2, 600);
    polaCen->setZValue(1);
    agregar(polaCen);
}

void Nivel::cargarSprites()
{
    spriteTecho = recortarTransparencia(QPixmap(":/imagenes/techo.png"));
    spritesLadrillo = cortarEnBandas(QPixmap(":/imagenes/plataformas.png"));

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

    escena->addItem(imagen);
    plataformas.append(imagen);
}

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

// ACCESO

QGraphicsScene *Nivel::obtenerEscena() const { return escena; }
Policarpa *Nivel::pola() const { return m_pola; }
int Nivel::numero() const { return m_numero; }
double Nivel::tiempoRestante() const { return tiempoRestanteSeg; }
bool Nivel::terminado() const { return fin; }

// CONTROL DEL NIVEL

void Nivel::iniciar()
{
    if (!m_pola && !polaCen)
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

    if (m_numero == 2)
    {
        for (Entidad *e : entidades)
            e->actualizar(dt);

        reglas(dt);
        emit actualizado();
        return;                 // no ejecuta lo del nivel 1
    }

    double piesAntes = m_pola->pies();

    for (Entidad *e : entidades)
        e->actualizar(dt);

    resolverPlataformas(piesAntes);
    resolverCombate();
    recogerPergaminos();
    revisarBuzon();

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
    case 2:
        reglasNivel2(dt);
        break;
    }
}

void Nivel::reglasNivel1(double dt)
{
    tiempoRestanteSeg -= dt;

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

void Nivel::reglasNivel2(double dt)
{

    Q_UNUSED(dt);

    if (!polaCen->estaVivo())
    {
        terminar(false);
        return;
    }

    // Aquí irá la condición de victoria del nivel 2
}

// GUARDIAS

void Nivel::agregarSoldado(int indicePlataforma)
{
    if (indicePlataforma < 0 || indicePlataforma >= plataformas.size())
    {
        qDebug() << "agregarSoldado: no existe la plataforma" << indicePlataforma;
        return;
    }

    QRectF piso = rectanguloPiso(plataformas[indicePlataforma]);

    QVector<QPointF> ruta;
    ruta << QPointF(piso.left(), piso.top())
         << QPointF(piso.right() - Soldado::ANCHO, piso.top());

    Soldado *soldado = new Soldado(ruta);
    agregar(soldado);
    soldados.append(soldado);
}

// COMBATE

void Nivel::resolverCombate()
{
    QRectF zona = m_pola->zonaAtaque();

    if (!zona.isNull())
    {
        for (int i = 0; i < soldados.size(); ++i)
        {
            if (soldados[i]->estaVivo() && zona.intersects(soldados[i]->sceneBoundingRect()))
                soldados[i]->recibirGolpe();
        }
    }

    for (int i = 0; i < soldados.size(); ++i)
    {
        if (soldados[i]->estaVivo() && m_pola->colisionaCon(soldados[i]))
            m_pola->recibirDanio();
    }

    for (int i = soldados.size() - 1; i >= 0; --i)
    {
        Soldado *soldado = soldados[i];

        if (!soldado->estaVivo())
        {
            soldados.removeAt(i);
            entidades.removeOne(soldado);
            escena->removeItem(soldado);
            soldado->deleteLater();
        }
    }
}

//  MENSAJES

void Nivel::agregarPergamino(int indicePlataforma, double posicion)
{
    if (indicePlataforma < 0 || indicePlataforma >= plataformas.size())
    {
        qDebug() << "agregarPergamino: no existe la plataforma" << indicePlataforma;
        return;
    }

    QRectF piso = rectanguloPiso(plataformas[indicePlataforma]);

    Pergamino *p = new Pergamino();

    double ancho = p->boundingRect().width();
    double alto = p->boundingRect().height();

    double x = piso.left() + (piso.width() - ancho) * posicion;
    double y = piso.top() - alto - ALTURA_PERGAMINO;

    p->setPos(x, y);
    agregar(p);
    pergaminos.append(p);
    ++total;
}

int Nivel::pergaminosRecogidos() const { return recogidos; }
int Nivel::totalPergaminos() const { return total; }

void Nivel::recogerPergaminos()
{
    for (int i = pergaminos.size() - 1; i >= 0; --i)
    {
        Pergamino *p = pergaminos[i];

        if (m_pola->colisionaCon(p))
        {
            escena->removeItem(p);
            entidades.removeOne(p);
            pergaminos.removeAt(i);
            p->deleteLater();

            ++recogidos;
            emit pergaminoRecogido(recogidos);
        }
    }
}

void Nivel::agregarBuzon(int indicePlataforma)
{
    if (indicePlataforma < 0 || indicePlataforma >= plataformas.size())
    {
        qDebug() << "agregarBuzon: no existe la plataforma" << indicePlataforma;
        return;
    }

    QPixmap hoja(":/imagenes/buzon.png");

    if (hoja.isNull())
    {
        qDebug() << "No se encontró :/imagenes/buzon.png (revisa el .qrc)";
        return;
    }

    int anchoCuadro = hoja.width() / 2;

    for (int i = 0; i < 2; ++i)
    {
        QPixmap cuadro = hoja.copy(i * anchoCuadro, 0, anchoCuadro, hoja.height());
        cuadrosBuzon.append(cuadro.scaledToHeight(ALTO_BUZON, Qt::FastTransformation));
    }

    QRectF piso = rectanguloPiso(plataformas[indicePlataforma]);

    buzon = new QGraphicsPixmapItem(cuadrosBuzon[0]);
    buzon->setPos(piso.center().x() - buzon->pixmap().width() / 2.0,
                  piso.top() - buzon->pixmap().height());
    buzon->setZValue(3);
    escena->addItem(buzon);
}

void Nivel::revisarBuzon()
{
    if (!buzon || buzonActivado || cuadrosBuzon.size() < 2)
        return;

    bool tocando = m_pola->sceneBoundingRect().intersects(buzon->sceneBoundingRect());

    if (tocando && recogidos == total)
    {
        buzonActivado = true;
        buzon->setPixmap(cuadrosBuzon[1]);   // se abre
        terminar(true);                      // victoria
    }
}