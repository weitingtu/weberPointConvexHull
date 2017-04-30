#include "mainwindow.h"
#include "panel.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDockWidget>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    _file_menu(nullptr),
    _view_menu(nullptr),
    _clear_act(nullptr),
    _zoom_in_act(nullptr),
    _zoom_out_act(nullptr),
    _zoom_fit_act(nullptr),
    _scene(new QGraphicsScene(this)),
    _view(new QGraphicsView(_scene, this)),
    _panel(new Panel(this)),
    _dock(new QDockWidget(tr("Control Panel"), this))
{
    setCentralWidget(_view);
    _create_dock_widget();
    _create_actions();
    _create_menus();
    _connect_panel();
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

void MainWindow::_create_dock_widget()
{
    _dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _dock->setWidget(_panel);
    addDockWidget(Qt::RightDockWidgetArea, _dock);
}

void MainWindow::_create_actions()
{
    _clear_act   = new QAction(tr("&Clear"), this);
    connect(_clear_act, SIGNAL(triggered(bool)), this, SLOT(_clear()));

    _zoom_in_act      = new QAction(tr("Zoom in"), this);
    _zoom_in_act->setShortcut(tr("Z"));
    connect(_zoom_in_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_in()));
    _zoom_out_act     = new QAction(tr("Zoom out"), this);
    _zoom_out_act->setShortcut(tr("Shift+Z"));
    connect(_zoom_out_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_out()));
    _zoom_fit_act     = new QAction(tr("Zoom fit"), this);
    _zoom_fit_act->setShortcut(tr("F"));
    connect(_zoom_fit_act, SIGNAL(triggered(bool)), this, SLOT(_zoom_fit()));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("File"));
    _file_menu->addAction(_clear_act);
    _file_menu->addSeparator();
    _view_menu = menuBar()->addMenu(tr("View"));
    _view_menu->addAction(_dock->toggleViewAction());
    _view_menu->addSeparator();
    _view_menu->addAction(_zoom_in_act);
    _view_menu->addAction(_zoom_out_act);
    _view_menu->addAction(_zoom_fit_act);
}

void MainWindow::_connect_panel()
{
    connect(_panel->get_clear_button(), SIGNAL(clicked(bool)), this, SLOT(_clear()));
}

void MainWindow::_clear()
{
    _scene->clear();
}

void MainWindow::_zoom_in()
{
    _view->scale(2.0, 2.0);
}

void MainWindow::_zoom_out()
{
    _view->scale(1.0 / 2.0, 1.0 / 2.0);
}

void MainWindow::_zoom_fit()
{
    _view->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
    _view->centerOn(_scene->sceneRect().center());
}
