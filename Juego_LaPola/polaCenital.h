#ifndef POLACENITAL_H
#define POLACENITAL_H

#include "Personaje.h"
#include <QPixmap>
#include <QVector>
#include <QSet>

class PolaCenital : public Personaje
{
    Q_OBJECT
public:
    enum Direccion { Arriba, Abajo, Izquierda, Derecha };

    PolaCenital();

    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    void actualizar(double dt) override;

private:
    QVector<QPixmap> cargarTira(const QString &ruta, int cuadros, bool espejo = false);
    void mover(int dx, int dy);
    bool chocaConPared() const;
    void actualizarSprite(bool moviendo);

    QVector<QPixmap> reposoFrente, reposoAtras;
    QVector<QPixmap> caminarArriba, caminarAbajo, caminarDerecha, caminarIzquierda;

    QSet<int> teclas;                        // teclas actualmente presionadas
    Direccion direccion = Abajo;
    int frame = 0;
    int contadorAnim = 0;
    int paso = 3;    // px por tick

    static const int ANCHO_POLA = 48;
    static const int ALTO_POLA  = 72;
};

#endif