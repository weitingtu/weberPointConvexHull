#ifndef CONVEXHULLMANAGER_H
#define CONVEXHULLMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>

class ConvexHullManager : public QObject
{
    Q_OBJECT
public:
    static ConvexHullManager& get_inst()
    {
        static ConvexHullManager inst;
        return inst;
    }

    void set_points(const QVector<QPointF>& points);
    void clear();
    void convex_hull();

    const QVector<QPointF>& get_convex_hull() const { return _convex_hull; }
    const QVector<QVector<QPointF> >& get_group() const { return _group; }
    const QVector<QVector<int> >&     get_group_idx() const { return _group_idx; }
    const QVector<int>& get_points_group_idx() const { return _points_group_idx; }

private:
    explicit ConvexHullManager(QObject *parent = 0);

private:

    QVector<QPointF>           _points;
    QVector<int>               _points_group_idx;
    QVector<QVector<QPointF> > _group;
    QVector<QVector<int> >     _group_idx;
    QVector<QPointF>           _convex_hull;
};

inline ConvexHullManager& get_convex_hull_manager()
{
    return ConvexHullManager::get_inst();
}


#endif // CONVEXHULLMANAGER_H
