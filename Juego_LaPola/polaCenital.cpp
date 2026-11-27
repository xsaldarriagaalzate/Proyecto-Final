#include "PolaCenital.h"
#include <QGraphicsScene>
#include <QTransform>
#include <QDebug>


static const int CUADROS_REPOSO   = 8;
static const int CUADROS_CAMINAR  = 8;
static const int TICKS_POR_CUADRO = 8;       // 8 * 16 ms = ~128 ms por cuadro

PolaCenital::PolaCenital() : Personaje(":/imagenes/pola_reposo.png", ANCHO_POLA, ALTO_POLA)
{
    reposoFrente     = cargarTira(":/imagenes/pola_reposo.png",         CUADROS_REPOSO);
    reposoAtras      = cargarTira(":/imagenes/pola_reposo_detras.png",   CUADROS_REPOSO);
    caminarArriba    = cargarTira(":/imagenes/pola_caminar_arriba.png", CUADROS_CAMINAR);
    caminarAbajo     = cargarTira(":/imagenes/pola_caminar_abajo.png",  CUADROS_CAMINAR);
    caminarDerecha   = cargarTira(":/imagenes/pola_caminar_lados.png",  CUADROS_CAMINAR);
    caminarIzquierda = cargarTira(":/imagenes/pola_caminar_lados.png",  CUADROS_CAMINAR, true);

    if (!reposoFrente.isEmpty())
        setPixmap(reposoFrente[0]);
}

QVector<QPixmap> PolaCenital::cargarTira(const QString &ruta, int cuadros, bool espejo)
{
    QVector<QPixmap> v;
    QPixmap hoja(ruta);
    if (hoja.isNull()) {
        qWarning() << "No se pudo cargar" << ruta;
        return v;
    }
    int ancho = hoja.width() / cuadros;
    for (int i = 0; i < cuadros; ++i) {
        QPixmap c = hoja.copy(i * ancho, 0, ancho, hoja.height());
        c = c.scaledToHeight(ALTO_POLA, Qt::FastTransformation);
        if (espejo)
            c = c.transformed(QTransform().scale(-1, 1));
        v.append(c);
    }
    return v;
}

void PolaCenital::teclaPresionada(int tecla)
{
    teclas.insert(tecla);

    if (tecla == Qt::Key_P)      // P = imprimir posición
    {
        QRectF cuerpo = sceneBoundingRect();
        qDebug() << "pos:" << pos()
                 << "| pies (centro-abajo):" << QPointF(cuerpo.center().x(), cuerpo.bottom());
    }
}

// void PolaCenital::teclaPresionada(int tecla) { teclas.insert(tecla); }
void PolaCenital::teclaSoltada(int tecla)    { teclas.remove(tecla); }

void PolaCenital::actualizar(double dt)
{
    Q_UNUSED(dt);
    int dx = 0, dy = 0;
    if (teclas.contains(Qt::Key_A)) dx -= 1;
    if (teclas.contains(Qt::Key_D)) dx += 1;
    if (teclas.contains(Qt::Key_W)) dy -= 1;
    if (teclas.contains(Qt::Key_S)) dy += 1;

    bool moviendo = (dx != 0 || dy != 0);

    if (moviendo) {
        // en diagonal gana el eje vertical para elegir el sprite
        if (dy < 0)      direccion = Arriba;
        else if (dy > 0) direccion = Abajo;
        else if (dx < 0) direccion = Izquierda;
        else             direccion = Derecha;

        mover(dx * paso, 0);                 // primero X
        mover(0, dy * paso);                 // luego Y
    }
    actualizarSprite(moviendo);
}

void PolaCenital::mover(int dx, int dy)
{
    if (dx == 0 && dy == 0) return;
    setPos(x() + dx, y() + dy);
    if (chocaConPared())
        setPos(x() - dx, y() - dy);
}

bool PolaCenital::chocaConPared() const
{
    if (!scene()) return false;
    const QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *it : items)
        if (it->data(0).toString() == "pared")
            return true;
    return false;
}

void PolaCenital::actualizarSprite(bool moviendo)
{
    const QVector<QPixmap> *tira = nullptr;

    if (!moviendo) {
        frame = 0;
        contadorAnim = 0;
        tira = (direccion == Arriba) ? &reposoAtras : &reposoFrente;
    } else {
        switch (direccion) {
        case Arriba:    tira = &caminarArriba;    break;
        case Abajo:     tira = &caminarAbajo;     break;
        case Izquierda: tira = &caminarIzquierda; break;
        case Derecha:   tira = &caminarDerecha;   break;
        }
        if (!tira->isEmpty() && ++contadorAnim >= TICKS_POR_CUADRO) {
            contadorAnim = 0;
            frame = (frame + 1) % tira->size();   // nunca se sale del rango
        }
    }

    if (tira && !tira->isEmpty())
        setPixmap((*tira)[frame % tira->size()]);
}