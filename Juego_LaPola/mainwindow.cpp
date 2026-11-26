#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#include <QSet>
#include <QDebug>
#include <QtMath>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("La Pola: Mensajera de la Libertad");
    setFixedSize(800, 600);

    // VISTA

    vista = new QGraphicsView(this);
    vista->setFrameShape(QFrame::NoFrame);
    vista->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->resetTransform();
    setCentralWidget(vista);

    // HUD (tiempo y vidas)

    hud = new QLabel(this);
    hud->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 150);"
                       "padding: 6px; font-size: 16px;");
    hud->move(10, 10);
    hud->hide();

    // MENÚ DE INICIO

    crearMenu();
    vista->setScene(escenaMenu);
}

// MENÚ

void MainWindow::crearMenu()
{
    const int W = 800, H = 600;

    escenaMenu = new QGraphicsScene(this);
    escenaMenu->setSceneRect(0, 0, W, H);

    QPixmap fondo(":/imagenes/fondo_inicio.png");
    if (!fondo.isNull())
    {
        escenaMenu->addPixmap(fondo.scaled(W, H, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    else
    {
        qDebug() << "No se encontró :/imagenes/inicio.png (revisa el .qrc)";
        escenaMenu->setBackgroundBrush(Qt::black);
    }

    QGraphicsTextItem *titulo = escenaMenu->addText("La Pola: Mensajera de la Libertad");
    titulo->setDefaultTextColor(Qt::black);
    titulo->setFont(QFont("Arial", 28, QFont::Bold));
    titulo->setPos((W - titulo->boundingRect().width()) / 2, 120);


    QPushButton *btn1 = new QPushButton("Nivel 1");
    btn1->setFixedSize(300, 50);
    btn1->setFocusPolicy(Qt::NoFocus);
    QGraphicsProxyWidget *proxy1 = escenaMenu->addWidget(btn1);
    proxy1->setPos((W - 300) / 2, 220);

    connect(btn1, &QPushButton::clicked, this, [this]() { cargarNivel(1); });
}

// CARGAR UN NIVEL

void MainWindow::cargarNivel(int numero)
{

    if (nivel)
    {
        vista->setScene(escenaMenu);
        pola = nullptr;
        nivel->deleteLater();
        nivel = nullptr;
    }

    teclasPresionadas.clear();

    // CREAR EL NIVEL

    nivel = new Nivel(numero, this);
    pola = nivel->pola();

    vista->setScene(nivel->obtenerEscena());


    connect(nivel, &Nivel::actualizado, this, [this]()
            {
                if (!nivel || !pola)
                    return;

                double centroX = pola->x() + pola->boundingRect().width() / 2.0;
                vista->centerOn(centroX, 300);

                if (!nivel->terminado())
                    actualizarHud();
            });

    connect(nivel, &Nivel::victoria, this, [this]()
            {
                mostrarMensaje("¡Mensaje entregado! Ganaste");

                QTimer::singleShot(3000, this, [this]()
                                   {
                                       if (nivel)
                                           volverAlMenu();
                                   });
            });

    connect(nivel, &Nivel::derrota, this, [this]()
            {
                if (nivel->tiempoRestante() <= 0)
                    mostrarMensaje("Se acabó, la misión fracasó");
                else
                    mostrarMensaje("Te quedaste sin vidas");

                QTimer::singleShot(3000, this, [this]()
                                   {
                                       if (nivel)
                                           volverAlMenu();
                                   });
            });

    hud->show();
    hud->raise();
    actualizarHud();

    nivel->iniciar();
    setFocus();

    // ACTUALIZAR ESCENARIO

    QTimer::singleShot(0, this, [this]()
                       {
                           vista->horizontalScrollBar()->setValue(0);
                           vista->verticalScrollBar()->setValue(0);
                           vista->viewport()->update();
                       });
}

// VOLVER AL MENÚ

void MainWindow::volverAlMenu()
{
    vista->setScene(escenaMenu);
    hud->hide();
    teclasPresionadas.clear();

    if (nivel)
    {
        pola = nullptr;
        nivel->deleteLater();
        nivel = nullptr;
    }

    vista->horizontalScrollBar()->setValue(0);
    vista->verticalScrollBar()->setValue(0);
    vista->viewport()->update();
}

// HUD

void MainWindow::actualizarHud()
{
    if (!pola || !nivel)
        return;

    int segundos = qCeil(nivel->tiempoRestante());

    hud->setText(QString("Tiempo: %1:%2    Vidas: %3")
                     .arg(segundos / 60)
                     .arg(segundos % 60, 2, 10, QChar('0'))
                     .arg(pola->getVidas()));
    hud->adjustSize();
}

void MainWindow::mostrarMensaje(const QString &texto)
{
    qDebug() << texto;
    hud->setText(texto);
    hud->adjustSize();
}

// CONTROL DEL TECLADO


void MainWindow::actualizarControles()
{
    if (!pola)
        return;

    // Movimiento horizontal (A tiene prioridad si se presionan ambas)
    if (teclasPresionadas.contains(Qt::Key_A))
        pola->moverIzquierda();
    else if (teclasPresionadas.contains(Qt::Key_D))
        pola->moverDerecha();
    else
        pola->detener();
}

void MainWindow::keyPressEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
        return;

    // Esc = abandonar el nivel y volver al menú
    if (evento->key() == Qt::Key_Escape && nivel)
    {
        volverAlMenu();
        return;
    }

    if (!pola)
        return;

    teclasPresionadas.insert(evento->key());

    actualizarControles();

    // Saltar
    if (evento->key() == Qt::Key_W || evento->key() == Qt::Key_Space)
    {
        pola->saltar();
    }

    // Atacar
    if (evento->key() == Qt::Key_J)
        pola->atacar();

}

// SOLTAR TECLAS

void MainWindow::keyReleaseEvent(QKeyEvent *evento)
{
    if (!pola || !nivel || evento->isAutoRepeat())
        return;

    teclasPresionadas.remove(evento->key());

    actualizarControles();
}

// DESTRUCTOR

MainWindow::~MainWindow()
{
    delete ui;
}