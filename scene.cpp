#include "scene.h"
#include "inputmanager.h"
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent ):
    QGraphicsScene(parent),
    _mode(MODE::RANDOM_INPUT),
    _texts()
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

void Scene::add_point(const QPointF& p,  const QPen &pen, double rad)
{
    double x = p.x();
    double y = p.y();
    addEllipse(x - rad, y - rad, rad * 2, rad * 2, pen);
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
