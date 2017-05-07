#include "decomposition.h"
#include <QLineF>

Decomposition::Decomposition(QObject *parent) : QObject(parent),
    _inputs(),
    _points(),
    _points_group_idx(),
    _triangles(),
    _target_idx(-1),
    _finish(false)
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
    _finish = false;
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
    _target_idx = -1;
    _finish = false;
}

bool Decomposition::is_valid() const
{
    if(_target_idx < 0)
    {
        return false;
    }
    return _target_idx < _triangles.size();
}

int Decomposition::_find_neighbor_triangle_idx(const Triangle& t, int idx1, int idx2) const
{
    for(int i = 0; i < 3; ++i)
    {
        if(t.neighbors[i] < 0)
        {
            continue;
        }
        const Triangle& tt = _triangles[t.neighbors[i]];
        bool found1 = false;
        bool found2 = false;
        for(int i = 0; i < 3; ++i)
        {
            if(tt.indices[i] == idx1)
            {
                found1 = true;
            }
            else if(tt.indices[i] == idx2)
            {
                found2 = true;
            }
        }
        if(found1 && found2)
        {
            return t.neighbors[i];
        }
    }
    return -1;
}

Triangle Decomposition::_create_triangle(const Triangle& t, const QPointF& c, int c_idx, int idx1, int idx2, int n_idx1, int n_idx2) const
{
    Triangle tt;
    tt.points[0]    = c;
    tt.indices[0]   = c_idx;
    tt.points[1]    = t.points[idx1];
    tt.indices[1]   = t.indices[idx1];
    tt.points[2]    = t.points[idx2];
    tt.indices[2]   = t.indices[idx2];
    tt.neighbors[0] = _find_neighbor_triangle_idx(t, t.indices[idx1], t.indices[idx2]);
    tt.neighbors[1] = n_idx1;
    tt.neighbors[2] = n_idx2;
    tt.idx          = t.idx;
    tt.set_center();
    _compute_weight(_inputs, tt);
    return tt;
}

void Decomposition::_decompose(const Triangle& t)
{
    int center_idx = _points.size();
    _points.push_back(t.center);
    _points_group_idx.push_back(t.idx);
    int t_idx1 = _triangles.size();
    int t_idx2 = _triangles.size() + 1;
    int t_idx3 = _triangles.size() + 2;
    _triangles.push_back(_create_triangle(t, t.center, center_idx, 0, 1, t_idx2, t_idx3));
    _triangles.push_back(_create_triangle(t, t.center, center_idx, 1, 2, t_idx3, t_idx1));
    _triangles.push_back(_create_triangle(t, t.center, center_idx, 2, 0, t_idx1, t_idx2));
}

void Decomposition::decompose()
{
    if(is_finish())
    {
        return;
    }
    if(!is_valid())
    {
        return;
    }
    _decompose(get_triangle());
    int idx       = _target_idx;
    double weight = get_triangle().weight;
    for(int i = _triangles.size() - 3;i <_triangles.size();++i)
    {
        if(_triangles[i].weight < weight)
        {
            weight = _triangles[i].weight;
            idx = i;
        }
    }
    if(idx == _target_idx)
    {
        _finish = true;
    }
    else
    {
        _target_idx = idx;
    }
}
