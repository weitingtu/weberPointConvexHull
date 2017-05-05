#include "inputmanager.h"
#include <stdlib.h>
#include <cmath>

static const int _default_width(600);
static const int _default_height(300);

InputManager::InputManager() :
    _width(_default_width),
    _height(_default_height),
    _inputs(),
    _hexs(),
    _convex_hull(),
    _boundary()
{
}

void InputManager::generate(int size)
{
    clear();
    for(int i = 0; i < size; ++i)
    {
        _inputs.push_back(QPointF(rand() % _width, rand() % _height));
    }
}

namespace  {

QRectF _find_boundary(const QVector<QPointF>& inputs)
{
    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double max_y = std::numeric_limits<double>::lowest();

    for(int i = 0; i < inputs.size(); ++i)
    {
        min_x = std::min(min_x, inputs[i].x());
        min_y = std::min(min_y, inputs[i].y());
        max_x = std::max(max_x, inputs[i].x());
        max_y = std::max(max_y, inputs[i].y());
    }
    return QRectF(QPointF(min_x, min_y), QPointF(max_x, max_y));
}

QVector<QPointF> _hexagonal(const QRectF &rect)
{
    double h = rect.height() / 3;
    double a = h / std::sqrt(3);
    double width  = rect.width();
    double height = rect.height();
    int x_ratio  = width / a;
    int y_ratio  = height / h;

    QVector<QPointF> points;
    for(int i = 0; i <  2 * y_ratio + 1; ++i)
    {
        for(int j = 0; j < x_ratio + 1; ++j)
        {
            double shift = 0;
            if(i % 2 == 1)
            {
                if(j == x_ratio)
                {
                    continue;
                }
                shift = a / 2;
            }
            double x = j * a + shift + rect.x();
            double y = i * h / 2 + rect.y();
            points.push_back(QPointF(x, y));
        }
    }
    return points;
}

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
QVector<QPointF> _get_convex_hull(QVector<QPointF> points)
{
    // Initialize Result
    QVector<QPointF> hull;

    // There must be at least 3 points
    if (points.size() < 3)
        return hull;

    // Find the leftmost point
    int l = 0;
    for (int i = 1; i < points.size(); i++)
        if (points[i].x() < points[l].x())
            l = i;

    // Start from leftmost point, keep moving counterclockwise
    // until reach the start point again.  This loop runs O(h)
    // times where h is number of points in result or output.
    int p = l, q;
    do
    {
        // Add current point to result
        hull.push_back(points[p]);

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
               q = i;
        }

        // Now q is the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // result 'hull'
        p = q;

    } while (p != l);  // While we don't come to first point

    return hull;
}

}

void InputManager::hexagonal()
{
    if(_inputs.size() < 4)
    {
        return;
    }
    _boundary = _find_boundary(_inputs);
    _hexs = _hexagonal(_boundary);
}

void InputManager::convex_hull()
{
    _convex_hull = _get_convex_hull(_inputs + _hexs);
}

void InputManager::clear()
{
    _inputs.clear();
    _boundary = QRectF();
    _hexs.clear();
    _convex_hull.clear();
}
