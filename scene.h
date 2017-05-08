#ifndef SCENE_H
#define SCENE_H

#include "enum.h"
#include <QPen>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

class Triangle;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject*parent = 0);

    void add_text(const QPointF& p, const QString& text);
    void clear_texts();
    void adjust_texts(int size);
    void clear_triangles();
    void add_triangle(const Triangle& t, const QPen& pen);

    void initialize();
    QGraphicsEllipseItem* add_point(const QPointF& p, const QPen &pen = QPen(), double rad = 1);

signals:

public slots:
    void set_mode(MODE m);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    MODE _mode;
    QVector<QGraphicsTextItem*> _texts;
    QVector<QGraphicsItem*>     _triangles;
};

#endif // SCENE_H
