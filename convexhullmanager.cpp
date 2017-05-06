#include "convexhullmanager.h"
#include <QLineF>
#include <QRectF>
#include <QStack>
#include <algorithm>
#include <vector>

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
//int orientation(QPointF p, QPointF q, QPointF r)
//{
//    double val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

//    if (val == 0) return 0;  // colinear
//    return (val > 0)? 1: 2;  // clock or counterclock wise
//}

// Prints convex hull of a set of n points.
//QVector<QPointF> _jarvis_convex_hull(const QVector<QPointF>& all_points, int idx, QVector<int>& group_idx )
//{
//    QVector<QPointF> points;
//    QVector<int>     points_idx;
//    for(int i = 0; i < all_points.size();++i)
//    {
//        if(group_idx[i] < 0)
//        {
//            points.push_back(all_points[i]);
//            points_idx.push_back(i);
//        }
//    }
//    // Initialize Result
//    QVector<QPointF> hull;
//    QVector<int>     hull_idx;

//    // There must be at least 3 points
//    if (points.size() < 3)
//    {
//        return hull;
//    }

//    // Find the leftmost point
//    int l = 0;
//    for (int i = 1; i < points.size(); i++)
//    {
//        if (points[i].x() < points[l].x())
//        {
//            l = i;
//        }
//        else if ((points[i].x() == points[l].x()) && points[i].y() < points[l].y())
//        {
//            l = i;
//        }
//    }

//    // Start from leftmost point, keep moving counterclockwise
//    // until reach the start point again.  This loop runs O(h)
//    // times where h is number of points in result or output.
//    int p = l;
//    int q = 0;
//    do
//    {
//        // Add current point to result
//        hull.push_back(points[p]);
//        hull_idx.push_back(p);

//        // Search for a point 'q' such that orientation(p, x,
//        // q) is counterclockwise for all points 'x'. The idea
//        // is to keep track of last visited most counterclock-
//        // wise point in q. If any point 'i' is more counterclock-
//        // wise than q, then update q.
//        q = (p + 1) % points.size();
//        for (int i = 0; i < points.size(); i++)
//        {
//           // If i is more counterclockwise than current q, then
//           // update q
//           if (orientation(points[p], points[i], points[q]) == 2)
//           {
//               q = i;
//           }
//        }

//        // Now q is the most counterclockwise with respect to p
//        // Set p as q for next iteration, so that q is added to
//        // result 'hull'
//        p = q;

//    } while (p != l);  // While we don't come to first point

//    for(int i = 0; i <hull_idx.size(); ++i)
//    {
//        group_idx[points_idx[hull_idx[i]]] = idx;
//    }
//    return hull;
//}

// A globle point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
//QPointF p0;

// A utility function to find next to top in a stack
//QPointF nextToTop(QStack<QPointF>& S)
//{
//    QPointF p = S.top();
//    S.pop();
//    QPointF res = S.top();
//    S.push(p);
//    return res;
//}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
//int compare(const void *vp1, const void *vp2)
//{
//   Point *p1 = (Point *)vp1;
//   Point *p2 = (Point *)vp2;

//   // Find orientation
//   int o = orientation(p0, *p1, *p2);
//   if (o == 0)
//     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

//   return (o == 2)? -1: 1;
//}

//bool lessThan(const QPointF& p1, const QPointF& p2)
//{
//   // Find orientation
//   int o = orientation(p0, p1, p2);
//   if (o == 0)
//   {
//         return QLineF(p0, p2).length() >= QLineF(p0, p1).length();
//   }

//   return o == 2;
//}

// Prints convex hull of a set of n points.
//QVector<QPointF> _graham_scan_convex_hull(QVector<QPointF> points)
//{
//    QVector<QPointF> hull;
//    if(points.size() < 3 )
//    {
//        return hull;
//    }

//   // Find the bottommost point
//   double ymin = points[0].y();
//   int min = 0;
//   for (int i = 1; i < points.size(); i++)
//   {
//     double y = points[i].y();

//     // Pick the bottom-most or chose the left
//     // most point in case of tie
//     if ((y < ymin) || (ymin == y && points[i].x() < points[min].x()))
//     {
//        ymin = points[i].y();
//        min = i;
//     }
//   }

//   // Place the bottom-most point at first position
//   std::swap(points[0], points[min]);

//   // Sort n-1 points with respect to the first point.
//   // A point p1 comes before p2 in sorted ouput if p2
//   // has larger polar angle (in counterclockwise
//   // direction) than p1
//   p0 = points[0];
//   std::sort(points.begin() + 1, points.end(), lessThan);

//   // If two or more points make same angle with p0,
//   // Remove all but the one that is farthest from p0
//   // Remember that, in above sorting, our criteria was
//   // to keep the farthest point at the end when more than
//   // one points have same angle.
//   int m = 1; // Initialize size of modified array
//   for (int i=1; i<points.size(); ++i)
//   {
//       // Keep removing i while angle of i and i+1 is same
//       // with respect to p0
//       while (i < (points.size() - 1) && orientation(p0, points[i], points[i+1]) == 0)
//       {
//          i++;
//       }


//       points[m] = points[i];
//       m++;  // Update size of modified array
//   }

//   // If modified array of points has less than 3 points,
//   // convex hull is not possible
//   if (m < 3)
//   {
//       return hull;
//   }

//   // Create an empty stack and push first three points
//   // to it.
//   QStack<QPointF> S;
//   S.push(points[0]);
//   S.push(points[1]);
//   S.push(points[2]);

//   // Process remaining n-3 points
//   for (int i = 3; i < m; i++)
//   {
//      // Keep removing top while the angle formed by
//      // points next-to-top, top, and points[i] makes
//      // a non-left turn
//      while (orientation(nextToTop(S), S.top(), points[i]) != 2)
//      {
//         S.pop();
//      }
//      S.push(points[i]);
//   }

//   // Now stack has the output points, print contents of stack
//   while (!S.empty())
//   {
//       QPointF p = S.top();
//       cout << "(" << p.x << ", " << p.y <<")" << endl;
//       hull.push_back(p);
//       S.pop();
//   }
//   return hull;
//}

// Implementation of Andrew's monotone chain 2D convex hull algorithm.
// Asymptotic complexity: O(n log n).
// Practical performance: 0.5-1.0 seconds for n=1000000 on a 1GHz machine.

typedef double coord_t;   // coordinate type
typedef double coord2_t;  // must be big enough to hold 2*max(|coordinate|)^2

struct Point {
    Point() : x(0), y(0), idx(0) {}
    Point(coord_t xx, coord_t yy, int i) : x(xx), y(yy), idx(i) {}
    coord_t x, y;
    int idx;

    bool operator <(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

//bool lessThan(const QPointF& p1, const QPointF& p2)
//{
//    return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
//}

// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
//coord2_t cross(const QPointF& O, const QPointF& A, const QPointF& B)
//{
//    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
//}
coord2_t cross(const Point& O, const Point& A, const Point& B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
QVector<Point> _andrew_monotone_chain_convex_hull(QVector<Point> P)
{
    int n = P.size();
    int k = 0;
    if (n == 1) return P;
    QVector<Point> H(2*n);

    // Sort points lexicographically
    std::sort(P.begin(), P.end());

    // Build lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    // Build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    H.resize(k-1);
    return H;
}

QVector<QPointF> _andrew_monotone_chain_convex_hull_wrapper(const QVector<QPointF>& all_points, int idx, QVector<int>& group_idx )
{
    QVector<QPointF> result;
    QVector<Point> points;
    for(int i = 0; i < all_points.size(); ++i)
    {
        if(group_idx[i] < 0)
        {
            points.push_back(Point(all_points[i].x(), all_points[i].y(), i));
        }
    }
    if(points.size() < 3)
    {
        return result;
    }
    QVector<Point>   hull = _andrew_monotone_chain_convex_hull(points);
    for(int i = 0; i < hull.size(); ++i)
    {
        group_idx[hull[i].idx] = idx;
        result.push_back(QPointF(hull[i].x, hull[i].y));
    }
    return result;
}

}

void ConvexHullManager::convex_hull()
{
//    _convex_hull = _jarvis_convex_hull(_points, _group.size(), _group_idx);
    _convex_hull = _andrew_monotone_chain_convex_hull_wrapper(_points, _group.size(), _group_idx);
//    _convex_hull = _graham_scan_convex_hull(_points);
    _group.push_back(_convex_hull);
}

void ConvexHullManager::set_points(const QVector<QPointF>& points)
{
    _points = points;
    _group_idx.fill(-1, points.size());
    _group.clear();
}
