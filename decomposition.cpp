#include "decomposition.h"

Decomposition::Decomposition(QObject *parent) : QObject(parent)
{
}

void Decomposition::set_input(const QVector<QPointF>& i, const QVector<QPointF>& p, const QVector<int>& idx, const QVector<Triangle>& t)
{
    _inputs = i;
    _points = p;
    _points_group_idx = idx;
    _triangles = t;
}
