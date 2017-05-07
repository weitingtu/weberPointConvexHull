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
    void clear();

signals:

public slots:
private:
    explicit Decomposition(QObject *parent = 0);
private:
    void _compute_weight(const QVector<QPointF>& inputs, Triangle& t) const;

    QVector<QPointF>  _inputs;
    QVector<QPointF>  _points;
    QVector<int>      _points_group_idx;
    QVector<Triangle> _triangles;
};

inline Decomposition& get_decomposition()
{
    return Decomposition::get_inst();
}

#endif // DECOMPOSITION_H
