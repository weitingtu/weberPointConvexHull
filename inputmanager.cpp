#include "inputmanager.h"
#include <stdlib.h>

static const int _default_width(600);
static const int _default_height(300);

InputManager::InputManager() :
    _width(_default_width),
    _height(_default_height),
    _inputs(),
    _hexs(),
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
    int x_ratio  = width / a + 1;
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

void InputManager::clear()
{
    _inputs.clear();
    _boundary = QRectF();
    _hexs.clear();
}
