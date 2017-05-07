#ifndef CDTMANAGER_H
#define CDTMANAGER_H

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */
#define VOID int
#include "triangulation.h"

#include "triangle.h"
#include "poly.h"
#include <QObject>
#include <QPolygonF>
#include <QPointF>
#include <QLineF>
#include <QVector>

class CDTManager : public QObject
{
    Q_OBJECT
public:
    static CDTManager& get_inst()
    {
        static CDTManager inst;
        return inst;
    }

    void set_points(const QVector<QPointF>& p) { _points = p; }
    void set_points_group_idx(const QVector<int>& p) { _points_group_idx = p; }
    void set_group_idx(const QVector<QVector<int> >& g) { _group_idx = g; }
    void clear();
    void cdt();
    void fermat_point();

    const QVector<QLineF>&   get_lines() const { return _lines; }
    const QVector<Triangle>& get_triangles() const { return _triangles; }

    const QVector<Poly>&     get_graph() const { return _graph; }
signals:

public slots:
private:
    explicit CDTManager(QObject *parent = 0);

    void _group_to_segments();
    struct triangulateio _create_input() const;
    struct triangulateio _create_mid() const;
    void _set_lines_by_edges(const triangulateio& io);
    void _set_triangles(const triangulateio& io);

    // raw data
    QVector<QPointF>           _points;
    QVector<int>               _points_group_idx;
    QVector<QVector<int> >     _group_idx;
    QVector<QPair<int, int> >  _segments;

    // output
    QVector<QLineF>            _lines;
    QVector<Triangle>          _triangles;
    QVector<QVector<Triangle>> _triangles_idx;

    QVector<Poly>              _graph;
};

inline CDTManager& get_cdt_manager()
{
    return CDTManager::get_inst();
}

#endif // CDTMANAGER_H
