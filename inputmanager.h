#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QRectF>

class InputManager : public QObject
{
    Q_OBJECT
public:
    static InputManager& get_inst()
    {
        static InputManager inst;
        return inst;
    }

    double get_width() const { return _width; }
    double get_height() const { return _height; }
    const QRectF& get_boundary() const { return _boundary; }

    void clear();
    void generate(int size);
    void add_point(const QPointF& p) { _inputs.push_back(p); }
    void hexagonal();

    const QVector<QPointF>& get_inputs() const { return _inputs; }
    const QVector<QPointF>& get_hexs() const { return _hexs; }

private:
    InputManager();

private:

    int _width;
    int _height;
    QVector<QPointF> _inputs;
    QVector<QPointF> _hexs;
    QRectF _boundary;

};

inline InputManager& get_input_manager()
{
    return InputManager::get_inst();
}

#endif // INPUTMANAGER_H
