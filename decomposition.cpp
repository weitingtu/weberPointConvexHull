#include "decomposition.h"
#include <QLineF>

Decomposition::Decomposition(QObject *parent) : QObject(parent)
{
}

void Decomposition::_compute_weight(const QVector<QPointF>& inputs, Triangle& t) const
{
    t.weight = 0;
    for(int i = 0; i < inputs.size(); ++i)
    {
        t.weight += QLineF(inputs[i], t.center).length();
    }
}

void Decomposition::set_input(const QVector<QPointF>& i, const QVector<QPointF>& p, const QVector<int>& idx, const QVector<Triangle>& t)
{
    _inputs = i;
    _points = p;
    _points_group_idx = idx;
    _triangles = t;
    for(int i = 0; i <_triangles.size(); ++i)
    {
        _compute_weight(_inputs, _triangles[i]);
    }
}

void Decomposition::clear()
{
    _inputs.clear();
    _points.clear();
    _points_group_idx.clear();
    _triangles.clear();
}
