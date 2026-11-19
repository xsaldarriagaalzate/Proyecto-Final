#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>      // agregado
#include <QGraphicsView>       // agregado
#include <QKeyEvent>           // agregado
#include "policarpa.h"         // agregado

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:                                              // agregado
    void keyPressEvent(QKeyEvent *evento) override;     // agregado
    void keyReleaseEvent(QKeyEvent *evento) override;   // agregado

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escena;    // agregado
    QGraphicsView *vista;      // agregado
    Policarpa *pola;           // agregado
};
#endif // MAINWINDOW_H