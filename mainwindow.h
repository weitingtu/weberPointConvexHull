#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;
class Panel;
class QDockWidget;
class Scene;
class Triangle;
class QPen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private slots:
    void _change_font_size(int i);
    void _clear();
    void _generate();
    void _hexagoanl();
    bool _convex_hull();
    void _convex_hull_auto();
    void _cdt();
    void _decompose();
    void _accomplish();
    void _zoom_in();
    void _zoom_out();
    void _zoom_fit();

private:
    void _create_dock_widget();
    void _create_actions();
    void _create_menus();
    void _connect_panel();
    void _draw_input();
    void _init_cdt_display(const QVector<Triangle>& triangles);
    void _draw_cdt(int idx);
    void _draw_triangle(const Triangle& t, const QPen& pen);

    QMenu*          _file_menu;
    QMenu*          _view_menu;
    QAction*        _clear_act;
    QAction*        _generate_act;
    QAction*        _zoom_in_act;
    QAction*        _zoom_out_act;
    QAction*        _zoom_fit_act;
    Scene*          _scene;
    QGraphicsView*  _view;
    Panel*          _panel;
    QDockWidget*    _dock;
    QVector<bool>   _cdt_display;
    int _con_secs;
    int _dec_secs;
    int _total_secs;
};

#endif // MAINWINDOW_H
