#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _scene(new QGraphicsScene(this)),
      _view(new QGraphicsView(_scene, this))
{
    setCentralWidget(_view);
}

MainWindow::~MainWindow()
{

}

QSize MainWindow::minimumSizeHint() const
{
    return QSize(800, 600);
}

QSize MainWindow::sizeHint() const
{
    return QSize(1024, 768);
}
