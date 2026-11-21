
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("La Pola: Mensajera de la Libertad");
    setFixedSize(800, 600);

    // CREAR ESCENARIO

    escena = new QGraphicsScene(this);

    QPixmap hojaFondos(":/imagenes/fondos_nivel-1.png");

    if (hojaFondos.isNull())
    {
        qDebug() << "Error: no se encontró el fondo.";
        return;   // pola queda en nullptr y los eventos de teclado lo comprueban
    }

    int anchoOrig = hojaFondos.width();
    int altoFranja = hojaFondos.height() / 3;

    int xActual = 0;

    // CARGAR LOS FONDOS

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

    // CREAR PERSONAJE

    pola = new Policarpa();

    pola->setPos(100, 400);
    pola->setZValue(10);
    pola->setVisible(true);

    escena->addItem(pola);

    // CONFIGURAR LA CÁMARA

    vista = new QGraphicsView(escena, this);

    vista->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vista->setFocusPolicy(Qt::NoFocus);
    vista->resetTransform();

    setCentralWidget(vista);

    // ACTUALIZAR PERSONAJE

    QTimer *reloj = new QTimer(this);

    cronometro.start();

    connect(reloj, &QTimer::timeout, this, [this]()
            {
                // restart() devuelve los milisegundos desde la última vez y reinicia el conteo
                double dt = cronometro.restart() / 1000.0;

                // Si la ventana se congela (por ejemplo al arrastrarla), dt sería enorme
                // y la Pola atravesaría el piso. Se limita a 50 ms.
                if (dt > 0.05)
                    dt = 0.05;

                pola->actualizar(dt);

                double centroX = pola->x() + pola->boundingRect().width() / 2.0;

                vista->centerOn(centroX, 300);
            });

    reloj->start(16);

    // ACTUALIZAR ESCENARIO

    QTimer::singleShot(0, this, [this]()
                       {
                           vista->horizontalScrollBar()->setValue(0);
                           vista->verticalScrollBar()->setValue(0);

                           vista->viewport()->update();
                           escena->update();
                       });
}

// CONTROL DEL TECLADO

// Una sola función decide qué hace la Pola según las teclas que están presionadas
void MainWindow::actualizarControles()
{
    // Movimiento horizontal (A tiene prioridad si se presionan ambas)
    if (teclasPresionadas.contains(Qt::Key_A))
        pola->moverIzquierda();
    else if (teclasPresionadas.contains(Qt::Key_D))
        pola->moverDerecha();
    else
        pola->detener();

    // Agacharse mientras S esté presionada
    pola->agachar(teclasPresionadas.contains(Qt::Key_S));
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    if (!pola || evento->isAutoRepeat())
        return;

    teclasPresionadas.insert(evento->key());

    actualizarControles();

    // Saltar
    if (evento->key() == Qt::Key_W ||
        evento->key() == Qt::Key_Space)
    {
        pola->saltar();
    }
}

// SOLTAR TECLAS

void MainWindow::keyReleaseEvent(QKeyEvent *evento)
{
    if (!pola || evento->isAutoRepeat())
        return;

    teclasPresionadas.remove(evento->key());

    actualizarControles();
}

// DESTRUCTOR

MainWindow::~MainWindow()
{
    delete ui;
}