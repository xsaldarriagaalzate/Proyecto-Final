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

    void moverIzquierda();
    void moverDerecha();
    void detener();
    void saltar();

    double radioDeteccion() const;
    void setSueloY(double y);

    double pies() const;
    void aterrizar(double yPies);
    void setEnSuelo(bool valor);
    void setSueloActivo(bool valor);

private:
    bool agachada;
    bool enSuelo;
    double sueloY;
    bool sueloActivo;
    int direccion;

    // Valores del personaje
    static constexpr double VELOCIDAD = 180.0;       // px/s caminando
    static constexpr double FACTOR_AGACHADA = 0.5;   // se mueve a la mitad
    static constexpr double GRAVEDAD = 1200.0;       // px/s²
    static constexpr double IMPULSO_SALTO = 650.0;   // px/s (ágil: salta alto)
    static constexpr double RADIO_NORMAL = 250.0;    // distancia a la que la ven
    static constexpr double RADIO_AGACHADA = 100.0;  // sigilosa: la ven de más cerca
    static constexpr double ALTURA_DE_PIE = 96.0;    // alto en pantalla de la Pola de pie

    static QRect limitesVisibles(const QImage &imagen);
    QVector<QPixmap> cargarTira(const QString &ruta, int nCuadros, double alturaObjetivo,
                                double escalaFija = 0.0, double *escalaUsada = nullptr);
    void actualizarAnimacion(double dt);

    QVector<QPixmap> animQuieta;
    QVector<QPixmap> animCorrer;
    QVector<QPixmap> animSaltar;
    const QVector<QPixmap> *animActual;
    int cuadroActual;
    double tiempoCuadro;
    bool mirandoIzquierda;
    bool animRepite;

    static constexpr double DURACION_CUADRO = 0.10;   // 0.10 s = 10 cuadros por segundo
};

#endif