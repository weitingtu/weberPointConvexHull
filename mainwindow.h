#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;
class Panel;
class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private slots:
    void _clear();
    void _generate();
    void _zoom_in();
    void _zoom_out();
    void _zoom_fit();

private:
    void _create_dock_widget();
    void _create_actions();
    void _create_menus();
    void _connect_panel();

    QMenu*          _file_menu;
    QMenu*          _view_menu;
    QAction*        _clear_act;
    QAction*        _generate_act;
    QAction*        _zoom_in_act;
    QAction*        _zoom_out_act;
    QAction*        _zoom_fit_act;
    QGraphicsScene* _scene;
    QGraphicsView*  _view;
    Panel*          _panel;
    QDockWidget*    _dock;
};

#endif // MAINWINDOW_H
