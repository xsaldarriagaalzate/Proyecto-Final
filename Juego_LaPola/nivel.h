#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include "entidad.h"
#include "policarpa.h"

class Nivel : public QObject
{
    Q_OBJECT
public:
    explicit Nivel(int numero, QObject *padre = nullptr);
    ~Nivel() override = default;

    static bool existe(int numero) { return numero == 1; }

    QGraphicsScene *obtenerEscena() const;
    Policarpa *pola() const;
    int numero() const;
    double tiempoRestante() const;
    bool terminado() const;

    void iniciar();

    void penalizar(double segundos);

    void imprimirPlataformas() const;

signals:
    void actualizado();
    void victoria();
    void derrota();

private:

    void construirNivel1();

    void cargarSprites();
    void agregarTecho(double x, double ySuperficie, double ancho);
    void agregarLadrillo(double x, double ySuperficie, int tipo);    // parte de arriba (tipo 0, 1 o 2)

    void agregarPlataforma(const QPixmap &dibujo, double x, double ySuperficie, double margenLateral, double desfase, int tipo);
    QRectF rectanguloPiso(const QGraphicsPixmapItem *plataforma) const;

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
    QPixmap spriteTecho;
    QVector<QPixmap> spritesLadrillo;
    QVector<QGraphicsPixmapItem*> plataformas;   // cada plataforma es su dibujo; los datos van dentro (setData)
    QTimer reloj;
    QElapsedTimer cronometro;

    static constexpr bool MOSTRAR_PLATAFORMAS = true;
    static constexpr double GROSOR_PLATAFORMA = 20.0;

    static constexpr bool MODO_EDICION = true;

    static constexpr int DATO_TIPO = 0;
    static constexpr int DATO_MARGEN = 1;
    static constexpr int DATO_DESFASE = 2;

    static constexpr double ESCALA_LADRILLO = 2.0;     // cuánto se agranda cada pieza de plataformas.png
    static constexpr double MARGEN_TECHO = 0.25;      // fracción del ancho SIN piso a cada lado (las pendientes)
    static constexpr double MARGEN_LADRILLO = 0.04;
    static constexpr double DESFASE_TECHO = 0.0;       // px que el dibujo sube sobre la superficie donde se para
    static constexpr double DESFASE_LADRILLO = 0.0;

    static constexpr double TIEMPO_NIVEL1 = 150.0;   // 2 min 30 s
    static constexpr double MARGEN_META = 150.0;     // px antes del final del mapa
    static constexpr double CALLE_Y = 700.0;         // más abajo de esto ya cayó a la calle
    static constexpr double CASTIGO_CAIDA = 10.0;    // segundos que resta caer a la calle
    static constexpr double TOLERANCIA = 4.0;        // px de margen al detectar el aterrizaje
    static constexpr double MARGEN_PIES = 0.25;      // fracción del ancho que no cuenta a cada lado
};

#endif // NIVEL_H