#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include "entidad.h"
#include "policarpa.h"

class Nivel : public QObject
{
    Q_OBJECT
public:
    explicit Nivel(int numero, QObject *padre = nullptr);
    ~Nivel() override = default;

    QGraphicsScene *obtenerEscena() const;
    Policarpa *pola() const;
    int numero() const;
    double tiempoRestante() const;
    bool terminado() const;

    void iniciar();

    void penalizar(double segundos);

signals:
    void actualizado();
    void victoria();
    void derrota();

private:

    void construirNivel1();
    void agregarPlataforma(double x, double ySuperficie, double ancho);

    void reglas(double dt);
    void reglasNivel1(double dt);

    void resolverPlataformas(double piesAntes);
    void reaparecer();

    void agregar(Entidad *e);
    void terminar(bool gano);
    void tick();

    int m_numero;
    QGraphicsScene *escena;
    Policarpa *m_pola;
    double tiempoRestanteSeg;
    double metaX;
    bool fin;
    int ultimaPlataforma;
    QPointF puntoSeguro;

    QVector<Entidad*> entidades;
    QVector<QRectF> plataformas;
    QTimer reloj;
    QElapsedTimer cronometro;

    static constexpr bool MOSTRAR_PLATAFORMAS = true;
    static constexpr double GROSOR_PLATAFORMA = 20.0;

    static constexpr double TIEMPO_NIVEL1 = 150.0;   // 2 min 30 s
    static constexpr double MARGEN_META = 150.0;     // px antes del final del mapa
    static constexpr double CALLE_Y = 700.0;         // más abajo de esto ya cayó a la calle
    static constexpr double CASTIGO_CAIDA = 10.0;    // segundos que resta caer a la calle
    static constexpr double TOLERANCIA = 4.0;        // px de margen al detectar el aterrizaje
    static constexpr double MARGEN_PIES = 0.25;      // fracción del ancho que no cuenta a cada lado
};

#endif // NIVEL_H