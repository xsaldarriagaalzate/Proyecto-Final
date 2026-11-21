
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSet>
#include <QKeyEvent>
#include <QElapsedTimer>
#include "policarpa.h"

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
    void actualizarControles();   // traduce las teclas presionadas en acciones de la Pola

    Ui::MainWindow *ui;
    QGraphicsScene *escena = nullptr;   // nullptr: seguro si el constructor termina antes
    QGraphicsView *vista = nullptr;
    Policarpa *pola = nullptr;
    QSet<int> teclasPresionadas;
    QElapsedTimer cronometro;           // mide el tiempo real entre frames
};
#endif // MAINWINDOW_H