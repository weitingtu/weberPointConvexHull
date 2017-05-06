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
    void convex_hull();

    const QVector<QPointF>& get_convex_hull() const { return _convex_hull; }

private:
    ConvexHullManager(QObject *parent = 0);

private:

    QVector<QPointF>           _points;
    QVector<int>               _group_idx;
    QVector<QVector<QPointF> > _group;
    QVector<QPointF> _convex_hull;
};

inline ConvexHullManager& get_convex_hull_manager()
{
    return ConvexHullManager::get_inst();
}


#endif // CONVEXHULLMANAGER_H
