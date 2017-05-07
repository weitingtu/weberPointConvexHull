#include "decomposition.h"
#include <QLineF>

Decomposition::Decomposition(QObject *parent) : QObject(parent),
    _inputs(),
    _points(),
    _points_group_idx(),
    _triangles(),
    _target_idx(-1)
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
    _target_idx = -1;
}

void Decomposition::initialize()
{
    int max_idx = 0;
    for(int i = 0; i <_triangles.size(); ++i)
    {
        _compute_weight(_inputs, _triangles[i]);
        max_idx = std::max(max_idx, _triangles[i].idx);
    }
    double min_weight = std::numeric_limits<double>::max();
    int idx = -1;
    for(int i = 0; i <_triangles.size(); ++i)
    {
        const Triangle&t = _triangles[i];
        if(t.idx < max_idx)
        {
            continue;
        }
        if(t.weight < min_weight)
        {
            min_weight = t.weight;
            idx = i;
        }
    }
    if(idx >= 0 )
    {
        _target_idx = idx;
    }
}

void Decomposition::clear()
{
    _inputs.clear();
    _points.clear();
    _points_group_idx.clear();
    _triangles.clear();
}

bool Decomposition::is_valid() const
{
    if(_target_idx < 0)
    {
        return false;
    }
    return _target_idx < _triangles.size();
}
