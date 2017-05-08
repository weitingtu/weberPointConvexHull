#include "mainwindow.h"
#include "panel.h"
#include "inputmanager.h"
#include "convexhullmanager.h"
#include "cdtmanager.h"
#include "decomposition.h"
#include "scene.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDockWidget>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QVector>
#include <QPointF>
#include <QPen>
#include <QMessageBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    _file_menu(nullptr),
    _view_menu(nullptr),
    _clear_act(nullptr),
    _generate_act(nullptr),
    _zoom_in_act(nullptr),
    _zoom_out_act(nullptr),
    _zoom_fit_act(nullptr),
    _scene(new Scene(this)),
    _view(new QGraphicsView(_scene, this)),
    _panel(new Panel(this)),
    _dock(new QDockWidget(tr("Control Panel"), this)),
    _cdt_display()
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
    return QSize(1600, 1050);
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
    _generate_act = new QAction(tr("&Generate"), this);
    connect(_generate_act, SIGNAL(triggered(bool)), this, SLOT(_clear()));

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
    connect(_panel->get_font_size_spin_box(), SIGNAL(valueChanged(int)), this, SLOT(_change_font_size(int)));
    connect(_panel->get_clear_button(), SIGNAL(clicked(bool)), this, SLOT(_clear()));
    connect(_panel->get_generate_button(), SIGNAL(clicked(bool)), this, SLOT(_generate()));
    connect(_panel->get_hex_button(), SIGNAL(clicked(bool)), this, SLOT(_hexagoanl()));
    connect(_panel->get_convex_hull_button(), SIGNAL(clicked(bool)), this, SLOT(_convex_hull()));
    connect(_panel->get_cdt_button(), SIGNAL(clicked(bool)), this, SLOT(_cdt()));
    connect(_panel->get_decompose_button(), SIGNAL(clicked(bool)), this, SLOT(_decompose()));
    connect(_panel->get_accomplish_button(), SIGNAL(clicked(bool)), this, SLOT(_accomplish()));
    connect(_panel, SIGNAL(mode_changed(MODE)), _scene, SLOT(set_mode(MODE)));
}

void MainWindow::_change_font_size(int i)
{
    _scene->adjust_texts(i);
}

void MainWindow::_clear()
{
    _scene->clear_texts();
    _scene->clear_triangles();
    _scene->clear();
    _cdt_display.clear();
    get_input_manager().clear();
    get_convex_hull_manager().clear();
    get_cdt_manager().clear();
    get_decomposition().clear();
}

void MainWindow::_generate()
{
    _clear();
    get_input_manager().generate(_panel->get_input_size());
    _draw_input();
}

void MainWindow::_hexagoanl()
{
    get_input_manager().hexagonal();
    _scene->addRect(get_input_manager().get_boundary(), QPen(Qt::red, 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    const QVector<QPointF>& hexs = get_input_manager().get_hexs();
    for(int i = 0; i < hexs.size(); ++i)
    {
        _scene->add_point(hexs[i], QPen(QColor(Qt::green)));
    }

    get_convex_hull_manager().set_points(get_input_manager().get_inputs() + get_input_manager().get_hexs());
}

void MainWindow::_convex_hull()
{
    static int count = 0;
    get_convex_hull_manager().convex_hull();
    QVector<QPointF> convex_hull = get_convex_hull_manager().get_convex_hull();
    if(convex_hull.empty())
    {
        return;
    }
    convex_hull.push_back(convex_hull.first());
    for(int i = 0; i < convex_hull.size() - 1; ++i)
    {
        if(count % 2 == 0)
        {
            _scene->add_point(convex_hull[i], QPen(QColor(Qt::darkGreen)));
        }
        else
        {
            _scene->add_point(convex_hull[i], QPen(QColor(Qt::darkBlue)), 2);
        }
        _scene->addLine(QLineF(convex_hull[i], convex_hull[i + 1]));
    }
    ++count;
}

void MainWindow::_cdt()
{
    get_cdt_manager().clear();
    get_cdt_manager().set_points(get_input_manager().get_inputs() + get_input_manager().get_hexs());
    get_cdt_manager().set_points_group_idx(get_convex_hull_manager().get_points_group_idx());
    get_cdt_manager().set_group_idx(get_convex_hull_manager().get_group_idx());
    get_cdt_manager().cdt();
    _init_cdt_display(get_cdt_manager().get_triangles());
    get_decomposition().set_input(get_input_manager().get_inputs(),
                                  get_input_manager().get_inputs() + get_input_manager().get_hexs(),
                                  get_convex_hull_manager().get_points_group_idx(),
                                  get_cdt_manager().get_triangles());
    get_decomposition().initialize();
    if(get_decomposition().is_valid())
    {
        for(int i = 0; i <get_decomposition().get_candidate_size(); ++i)
        {
            _draw_triangle(get_decomposition().get_candidate_triangle(i), QPen(QColor(Qt::darkRed)));
        }
        const Triangle& t = get_decomposition().get_triangle();
        _draw_triangle(t, QPen(QColor(Qt::red)));
    }
}

void MainWindow::_decompose()
{
    if(!get_decomposition().is_valid())
    {
        return;
    }

    const Triangle& prev_t = get_decomposition().get_triangle();

    get_decomposition().decompose();

    if(get_decomposition().is_finish())
    {
        QString msg = QString("Finish");
        QMessageBox::information(this, QString(), msg);
        return;
    }

    _scene->clear_texts();
    _scene->clear_triangles();
    const Triangle& t = get_decomposition().get_triangle();
    if(get_decomposition().get_candidate_size() == 0)
    {
        _draw_triangle(prev_t, QPen(QColor(Qt::darkRed)));
    }
    else
    {
        for(int i = 0; i <get_decomposition().get_candidate_size(); ++i)
        {
            _draw_triangle(get_decomposition().get_candidate_triangle(i), QPen(QColor(Qt::darkRed)));
        }
    }
    _draw_triangle(t, QPen(QColor(Qt::red)));
}

void MainWindow::_accomplish()
{
    _scene->clear_texts();
    _scene->clear_triangles();
    _scene->clear();
    _draw_input();
    const Triangle& t = get_decomposition().get_triangle();
    _scene->add_point(t.center, QPen(QColor(Qt::red)));
}

void MainWindow::_draw_input()
{
    const QVector<QPointF>& inputs = get_input_manager().get_inputs();
    for(int i = 0; i < inputs.size(); ++i)
    {
        _scene->add_point(inputs[i]);
    }
}

void MainWindow::_init_cdt_display(const QVector<Triangle>& triangles)
{
    int max_idx = 0;
    for(int i = 0; i < triangles.size();++i)
    {
        max_idx = std::max(max_idx, triangles[i].idx);
    }
    _cdt_display.resize(max_idx + 1);
    _cdt_display.fill(false);
}

void MainWindow::_draw_cdt(int idx)
{
    if(_cdt_display[idx])
    {
        return;
    }
    _cdt_display[idx] = true;
    const QVector<Triangle>& triangles   = get_cdt_manager().get_triangles();
    const QVector<int>& points_group_idx = get_convex_hull_manager().get_points_group_idx();
    for(int i = 0; i < triangles.size(); ++i)
    {
        const Triangle& t = triangles[i];
        if(t.idx != idx)
        {
            continue;
        }
        if(points_group_idx[t.indices[0]] == points_group_idx[t.indices[1]]
                && points_group_idx[t.indices[1]] == points_group_idx[t.indices[2]]
                && points_group_idx[t.indices[2]] == points_group_idx[t.indices[0]])
        {
            for(int i = 0; i < 3; ++i)
            {
                int idx1 = i;
                int idx2 = i + 1;
                if(3 <= idx2)
                {
                    idx2 = 0;
                }
                _scene->addLine(QLineF(t.points[idx1], t.points[idx2]), QPen(QColor(Qt::gray)));
            }
        }
        else
        {
            for(int i = 0; i < 3; ++i)
            {
                int idx1 = i;
                int idx2 = i + 1;
                if(3 <= idx2)
                {
                    idx2 = 0;
                }
                if(points_group_idx[t.indices[idx1]] != points_group_idx[t.indices[idx2]])
                {
                    _scene->addLine(QLineF(t.points[idx1], t.points[idx2]), QPen(QColor(Qt::gray)));
                }
            }
        }
    }
}

void MainWindow::_draw_triangle(const Triangle& t, const QPen& pen)
{
    _draw_cdt(t.idx);
    _scene->add_triangle(t, pen);
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
