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
#include <QGraphicsRectItem>
#include <QPen>

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

    // CONTADOR DE PERGAMINOS (arriba a la derecha)

    contador = new QLabel(this);
    contador->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 150);"
                            "padding: 6px; font-size: 16px;");
    contador->hide();

    // MENÚ DE INICIO

    crearMenu();
    crearHistoria();
    crearControles();
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

    connect(btn1, &QPushButton::clicked, this, [this]() { mostrarHistoria(1); });

    QPushButton *btn2 = new QPushButton("Nivel 2");
    btn2->setFixedSize(300, 50);
    btn2->setFocusPolicy(Qt::NoFocus);
    QGraphicsProxyWidget *proxy2 = escenaMenu->addWidget(btn2);
    proxy2->setPos((W - 300) / 2, 290);

    connect(btn2, &QPushButton::clicked, this, [this]() { mostrarHistoria(2); });
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
    numeroActual = numero;
    pola  = (numero == 1) ? nivel->pola()        : nullptr;
    polaC = (numero == 2) ? nivel->polaCenital() : nullptr;

    vista->setScene(nivel->obtenerEscena());

    connect(nivel, &Nivel::pergaminoRecogido, this, [this]()
            {
                actualizarContador();
            });

    connect(nivel, &Nivel::actualizado, this, [this]()
            {
                if (!nivel)
                    return;

                if (numeroActual == 1 && pola)
                {
                    double centroX = pola->x() + pola->boundingRect().width() / 2.0;
                    vista->centerOn(centroX, 300);
                }
                else if (numeroActual == 2 && polaC)
                {
                    vista->centerOn(polaC);      // en cenital sigue en X y en Y
                }

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
                if (numeroActual == 1 && nivel->tiempoRestante() <= 0)
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
    contador->show();
    contador->raise();
    actualizarHud();
    actualizarContador();

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
    contador->hide();
    teclasPresionadas.clear();

    if (nivel)
    {
        pola = nullptr;
        nivel->deleteLater();
        nivel = nullptr;
        polaC = nullptr;
    }

    vista->horizontalScrollBar()->setValue(0);
    vista->verticalScrollBar()->setValue(0);
    vista->viewport()->update();
}

// HUD
void MainWindow::actualizarHud()
{
    if (!nivel || (!pola && !polaC))
        return;

    int vidas = pola ? pola->getVidas() : polaC->getVidas();

    if (numeroActual == 2)
    {
        hud->setText(QString("Vidas: %1").arg(vidas));
    }
    else
    {
        int segundos = qCeil(nivel->tiempoRestante());
        hud->setText(QString("Tiempo: %1:%2    Vidas: %3")
                         .arg(segundos / 60)
                         .arg(segundos % 60, 2, 10, QChar('0'))
                         .arg(vidas));
    }
    hud->adjustSize();
}

void MainWindow::actualizarContador()
{
    if (!nivel)
        return;

    contador->setText(QString("Pergaminos: %1/%2")
                          .arg(nivel->pergaminosRecogidos())
                          .arg(nivel->totalPergaminos()));
    contador->adjustSize();
    contador->move(width() - contador->width() - 10, 10);
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

    if (vista->scene() == escenaHistoria)
    {
        if (evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter
            || evento->key() == Qt::Key_Space)
            vista->setScene(escenaControles);
        else if (evento->key() == Qt::Key_Escape)
            vista->setScene(escenaMenu);
        return;
    }

    if (vista->scene() == escenaControles)
    {
        if (evento->key() == Qt::Key_Return || evento->key() == Qt::Key_Enter
            || evento->key() == Qt::Key_Space)
            cargarNivel(nivelPendiente);
        else if (evento->key() == Qt::Key_Escape)
            vista->setScene(escenaHistoria);
        return;
    }

    // Esc = abandonar el nivel y volver al menú
    if (evento->key() == Qt::Key_Escape && nivel)
    {
        volverAlMenu();
        return;
    }

    if (polaC)
    {
        polaC->teclaPresionada(evento->key());
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
    if (!nivel || evento->isAutoRepeat())
        return;

    if (polaC)
    {
        polaC->teclaSoltada(evento->key());
        return;
    }

    if (!pola)
        return;

    teclasPresionadas.remove(evento->key());
    actualizarControles();
}

// PANTALLAS

QGraphicsScene *MainWindow::crearPantalla(const QString &titulo, const QString &html,
                                          const QString &textoBoton,
                                          std::function<void()> alPulsar)
{
    const int W = 800, H = 600;

    QGraphicsScene *s = new QGraphicsScene(this);
    s->setSceneRect(0, 0, W, H);

    QPixmap fondo(":/imagenes/fondo_inicio.png");
    if (!fondo.isNull())
        s->addPixmap(fondo.scaled(W, H, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    else
        s->setBackgroundBrush(Qt::black);

    // Panel semitransparente para que el texto se lea
    s->addRect(80, 40, 640, 520, QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 180)));

    QGraphicsTextItem *tit = s->addText(titulo);
    tit->setDefaultTextColor(Qt::white);
    tit->setFont(QFont("Arial", 26, QFont::Bold));
    tit->setPos((W - tit->boundingRect().width()) / 2, 60);

    QGraphicsTextItem *cuerpo = s->addText("");
    cuerpo->setHtml(html);
    cuerpo->setTextWidth(560);
    cuerpo->setPos(120, 130);

    QPushButton *btn = new QPushButton(textoBoton);
    btn->setFixedSize(220, 46);
    btn->setFocusPolicy(Qt::NoFocus);
    QGraphicsProxyWidget *proxy = s->addWidget(btn);
    proxy->setPos((W - 220) / 2, 470);
    connect(btn, &QPushButton::clicked, this, [alPulsar]() { alPulsar(); });

    QGraphicsTextItem *ayuda = s->addText("Enter / Espacio: continuar     Esc: volver");
    ayuda->setDefaultTextColor(QColor(200, 200, 200));
    ayuda->setFont(QFont("Arial", 10));
    ayuda->setPos((W - ayuda->boundingRect().width()) / 2, 525);

    return s;
}

void MainWindow::crearHistoria()
{
    QString html =
        "<div style='color:white; font-size:16px;'>"
        "<p><b>Santafé de Bogotá, 1817.</b></p>"
        "<p>Policarpa Salavarrieta, «La Pola», es costurera y espía de la causa patriota. "
        "Los soldados realistas vigilan los tejados y las calles del barrio.</p>"
        "<p>Hoy debe recoger los <b>10 mensajes</b> escondidos entre los tejados y "
        "llevarlos al <b>buzón</b> de su contacto antes de que se agote el tiempo.</p>"
        "<p>Si los guardias la atrapan perderá vidas. "
        "Si se queda sin vidas o sin tiempo, la misión fracasa.</p>"
        "</div>";

    escenaHistoria = crearPantalla("La misión", html, "Continuar",
                                   [this]() { vista->setScene(escenaControles); });
}

void MainWindow::crearControles()
{
    QString html =
        "<div style='color:white; font-size:17px;'>"
        "<table cellspacing='10'>"
        "<tr><td><b>A / D</b></td><td>Moverse a la izquierda / derecha</td></tr>"
        "<tr><td><b>W / Espacio</b></td><td>Saltar</td></tr>"
        "<tr><td><b>J</b></td><td>Atacar</td></tr>"
        "<tr><td><b>Esc</b></td><td>Volver al menú</td></tr>"
        "</table>"
        "<p>Recoge los 10 pergaminos y llega al buzón para ganar.</p>"
        "</div>";

    escenaControles = crearPantalla("Controles", html, "¡Comenzar!",
                                    [this]() { cargarNivel(nivelPendiente); });
}

void MainWindow::mostrarHistoria(int numero)
{
    nivelPendiente = numero;
    vista->setScene(escenaHistoria);
    setFocus();
}

// DESTRUCTOR

MainWindow::~MainWindow()
{
    delete ui;
}