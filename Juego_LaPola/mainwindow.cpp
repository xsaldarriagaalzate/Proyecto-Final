#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollBar>
#include <QTimer>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("La Pola: Mensajera de la Libertad");
    setFixedSize(800, 600);

    escena = new QGraphicsScene(this);

    QPixmap hojaFondos(":/imagenes/fondos_nivel-1.png");

    if (hojaFondos.isNull()) {
        qDebug() << "No se encontró el fondo";
        return;
    }

    int anchoOrig = hojaFondos.width();
    int altoFranja = hojaFondos.height() / 3;

    int xActual = 0;

    for (int i = 0; i < 3; ++i) {

        QPixmap franja = hojaFondos.copy(
            0,
            (i * altoFranja) + 4,
            anchoOrig,
            altoFranja - 8
            );

        QPixmap franjaEscalada =
            franja.scaledToHeight(600, Qt::FastTransformation);

        QGraphicsPixmapItem *itemFondo =
            new QGraphicsPixmapItem(franjaEscalada);

        itemFondo->setPos(xActual, 0);
        itemFondo->setZValue(-1);

        escena->addItem(itemFondo);

        xActual += franjaEscalada.width();
    }

    escena->setSceneRect(0, 0, xActual, 600);

    pola = new Policarpa();

    pola->setPos(100, 400);
    pola->setZValue(10);
    pola->setVisible(true);

    escena->addItem(pola);

    vista = new QGraphicsView(escena, this);

    vista->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->resetTransform();

    setCentralWidget(vista);

    QTimer::singleShot(0, this, [this]() {

        vista->horizontalScrollBar()->setValue(0);
        vista->verticalScrollBar()->setValue(0);

        vista->viewport()->update();
        escena->update();

    });
}


void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {

    case Qt::Key_A:
        pola->moverIzquierda();
        break;

    case Qt::Key_D:
        pola->moverDerecha();
        break;

    case Qt::Key_W:
    case Qt::Key_Space:
        pola->saltar();
        break;

    case Qt::Key_S:
        pola->agachar(true);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat()) return;

    switch (evento->key()) {

    case Qt::Key_A:
    case Qt::Key_D:
        pola->detener();
        break;

    case Qt::Key_S:
        pola->agachar(false);
        break;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}


