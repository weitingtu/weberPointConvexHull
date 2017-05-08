#include "scene.h"
#include "inputmanager.h"
#include "triangle.h"
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent ):
    QGraphicsScene(parent),
    _mode(MODE::RANDOM_INPUT),
    _texts(),
    _triangles()
{
    initialize();
}

void Scene::add_text(const QPointF& p, const QString& text)
{
    QGraphicsTextItem* text_item = addText(text);
    text_item->setX(p.x());
    text_item->setY(p.y());
    _texts.push_back(text_item);
}

void Scene::clear_texts()
{
    for(int i = 0; i < _texts.size(); ++i)
    {
        removeItem(_texts[i]);
        delete(_texts[i]);
    }
    _texts.clear();
}

void Scene::adjust_texts(int size)
{
    for(int i = 0; i < _texts.size(); ++i)
    {
        QFont f = _texts[i]->font();
        f.setPointSize(size);
        _texts[i]->setFont(f);
    }
}

void Scene::initialize()
{
    double width  = get_input_manager().get_width();
    double height = get_input_manager().get_height();
    setSceneRect(0, 0, width, height);
}

QGraphicsEllipseItem* Scene::add_point(const QPointF& p,  const QPen &pen, double rad)
{
    double x = p.x();
    double y = p.y();
    return addEllipse(x - rad, y - rad, rad * 2, rad * 2, pen);
}

void Scene::set_mode( MODE m)
{
    clear();
    get_input_manager().clear();
    _mode = m;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if( (MODE::MANUAL_INPUT == _mode )
            && ( mouseEvent->button()==Qt::LeftButton ) && (sceneRect().contains(mouseEvent->scenePos())) )
    {
        get_input_manager().add_point(mouseEvent->scenePos());
        add_point(mouseEvent->scenePos());
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::clear_triangles()
{
    for(int i = 0; i < _triangles.size(); ++i)
    {
        removeItem(_triangles[i]);
        delete(_triangles[i]);
    }
    _triangles.clear();
}

void Scene::add_triangle(const Triangle& t, const QPen& pen)
{
    _triangles.push_back(add_point(t.center, pen));
    for(int i = 0; i < 2; ++i)
    {
        _triangles.push_back(addLine(QLineF(t.points[i], t.points[i + 1]), pen));
    }
    _triangles.push_back(addLine(QLineF(t.points[2], t.points[0]), pen));
    add_text(t.center, QString::number(t.weight));
//    QGraphicsTextItem* text_item = addText(QString::number(t.weight));
//    text_item->setX(t.center.x());
//    text_item->setY(t.center.y());
//    _triangles.push_back(text_item);
}
