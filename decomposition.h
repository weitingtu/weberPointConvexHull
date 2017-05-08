#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include "triangle.h"
#include <QObject>
#include <QVector>
#include <QPointF>

class Decomposition : public QObject
{
    Q_OBJECT
public:
    static Decomposition& get_inst()
    {
        static Decomposition inst;
        return inst;
    }

    void set_input(const QVector<QPointF>& i, const QVector<QPointF>& p, const QVector<int>& idx, const QVector<Triangle>& t);
    void initialize();
    void decompose();
    void clear();

    bool is_valid() const;
    bool is_finish() const { return _finish; }
    const Triangle& get_triangle() const { return _triangles[_target_idx]; }
    int get_candidate_size() const { return _candidate_idx.size(); }
    const Triangle& get_candidate_triangle(int idx) const { return _triangles[_candidate_idx[idx]]; }
signals:

public slots:
private:
    explicit Decomposition(QObject *parent = 0);
private:
    void _compute_weight(const QVector<QPointF>& inputs, Triangle& t) const;
    int _find_neighbor_triangle_idx(const Triangle& t, int idx1, int idx2) const;
    Triangle _create_triangle(const Triangle &t, const QPointF& c, int c_idx, int idx1, int idx2, int n_idx1, int n_idx2) const;
    void _decompose(const Triangle& t);

    QVector<QPointF>  _inputs;
    QVector<QPointF>  _points;
    QVector<int>      _points_group_idx;
    QVector<Triangle> _triangles;
    QVector<int>      _candidate_idx;
    int               _target_idx;
    bool              _finish;
};

inline Decomposition& get_decomposition()
{
    return Decomposition::get_inst();
}

#endif // DECOMPOSITION_H
