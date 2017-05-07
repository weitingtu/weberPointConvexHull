#include "triangle.h"

Triangle::Triangle() :
    points(),
    indices(),
    neighbors(),
    idx(),
    center(),
    weight()
{
}

void Triangle::set_center()
{
    center = QPointF((points[0].x() + points[1].x() + points[2].x()) / 3, (points[0].y() + points[1].y() + points[2].y()) / 3);
}
