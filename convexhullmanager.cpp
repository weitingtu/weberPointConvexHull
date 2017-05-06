#include "convexhullmanager.h"

ConvexHullManager::ConvexHullManager(QObject *parent):
    QObject(parent),
    _points(),
    _group_idx(),
    _group(),
    _convex_hull()
{
}

namespace  {

// A C++ program to find convex hull of a set of points. Refer
// http://www.geeksforgeeks.org/orientation-3-ordered-points/
// for explanation of orientation()

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(QPointF p, QPointF q, QPointF r)
{
    double val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2;  // clock or counterclock wise
}

// Prints convex hull of a set of n points.
QVector<QPointF> _jarvis_convex_hull(const QVector<QPointF>& all_points, int idx, QVector<int>& group_idx )
{
    QVector<QPointF> points;
    QVector<int>     points_idx;
    for(int i = 0; i < all_points.size();++i)
    {
        if(group_idx[i] < 0)
        {
            points.push_back(all_points[i]);
            points_idx.push_back(i);
        }
    }
    // Initialize Result
    QVector<QPointF> hull;
    QVector<int>     hull_idx;

    // There must be at least 3 points
    if (points.size() < 3)
    {
        return hull;
    }

    // Find the leftmost point
    int l = 0;
    for (int i = 1; i < points.size(); i++)
    {
        if (points[i].x() < points[l].x())
        {
            l = i;
        }
    }

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = l, q;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);
        hull_idx.push_back(p);

        // Search for a point 'q' such that orientation(p, x,
        // q) is counterclockwise for all points 'x'. The idea
        // is to keep track of last visited most counterclock-
        // wise point in q. If any point 'i' is more counterclock-
        // wise than q, then update q.
        q = (p + 1) % points.size();
        for (int i = 0; i < points.size(); i++)
        {
           // If i is more counterclockwise than current q, then
           // update q
           if (orientation(points[p], points[i], points[q]) == 2)
           {
               q = i;
           }
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

    } while (p != l);  // While we don't come to first point

    for(int i = 0; i <hull_idx.size(); ++i)
    {
        group_idx[points_idx[hull_idx[i]]] = idx;
    }
    return hull;
}

}

void ConvexHullManager::convex_hull()
{
    _convex_hull = _jarvis_convex_hull(_points, _group.size(), _group_idx);
    _group.push_back(_convex_hull);
}

void ConvexHullManager::set_points(const QVector<QPointF>& points)
{
    _points = points;
    _group_idx.fill(-1, points.size());
    _group.clear();
}
