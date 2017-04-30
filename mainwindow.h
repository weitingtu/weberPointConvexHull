#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private:
    QGraphicsScene* _scene;
    QGraphicsView*  _view;
};

#endif // MAINWINDOW_H
