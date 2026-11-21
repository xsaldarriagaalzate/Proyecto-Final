#ifndef POLICARPA_H
#define POLICARPA_H

#include <QVector>
#include <QPixmap>
#include <QImage>
#include <QRect>

#include "personaje.h"


class Policarpa : public Personaje
{
    Q_OBJECT
public:
    Policarpa();

    void actualizar(double dt) override;

    // Acciones
    void moverIzquierda();
    void moverDerecha();
    void detener();
    void saltar();
    void agachar(bool valor);

    // Información para otras clases (guardias, Pacman)
    bool estaAgachada() const;
    double radioDeteccion() const;
    void setSueloY(double y);

private:
    bool agachada;
    bool enSuelo;
    double sueloY;      // altura del piso en la escena
    int direccion;      // intención de movimiento: -1 izquierda, 0 quieta, 1 derecha

    // Valores del personaje
    static constexpr double VELOCIDAD = 180.0;       // px/s caminando
    static constexpr double FACTOR_AGACHADA = 0.5;   // se mueve a la mitad
    static constexpr double GRAVEDAD = 1200.0;       // px/s²
    static constexpr double IMPULSO_SALTO = 650.0;   // px/s (ágil: salta alto)
    static constexpr double RADIO_NORMAL = 250.0;    // distancia a la que la ven
    static constexpr double RADIO_AGACHADA = 100.0;  // sigilosa: la ven de más cerca
    static constexpr double ALTURA_DE_PIE = 96.0;    // alto en pantalla de la Pola de pie
    static constexpr double ALTURA_AGACHADA = 64.0;  // alto en pantalla de la Pola agachada

    static QRect limitesVisibles(const QImage &imagen);
    QVector<QPixmap> cargarTira(const QString &ruta, int nCuadros, double alturaObjetivo,
                                double escalaFija = 0.0, double *escalaUsada = nullptr);
    void actualizarAnimacion(double dt);

    QVector<QPixmap> animQuieta;
    QVector<QPixmap> animCorrer;
    QVector<QPixmap> animSaltar;
    QVector<QPixmap> animAgachada;
    QVector<QPixmap> animAgachadaQuieta;
    const QVector<QPixmap> *animActual;   // cuál animación se está mostrando
    int cuadroActual;
    double tiempoCuadro;                  // tiempo acumulado en el cuadro actual
    bool mirandoIzquierda;
    bool animRepite;

    static constexpr double DURACION_CUADRO = 0.10;   // 0.10 s = 10 cuadros por segundo
};

#endif