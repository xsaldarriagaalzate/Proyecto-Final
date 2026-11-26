#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QSet>
#include <QKeyEvent>
#include <QGraphicsScene>
#include "nivel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *evento) override;
    void keyReleaseEvent(QKeyEvent *evento) override;

private:

    void crearMenu();
    void cargarNivel(int numero);
    void volverAlMenu();

    QGraphicsScene *escenaMenu = nullptr;

    void actualizarControles();
    void actualizarHud();  // tiempo y vidas
    void mostrarMensaje(const QString &texto);

    Ui::MainWindow *ui;
    Nivel *nivel = nullptr;
    QGraphicsView *vista = nullptr;
    Policarpa *pola = nullptr;
    QLabel *hud = nullptr;
    QSet<int> teclasPresionadas;

    QLabel *contador = nullptr;
    void actualizarContador();
};
#endif // MAINWINDOW_H