#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QVector>
#include <QPointF>

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

    void clear();
    void generate(int size);
    void add_point(const QPointF& p) { _inputs.push_back(p); }
    void hexagonal();

    const QVector<QPointF>& get_inputs() const { return _inputs; }

private:
    InputManager();

    int _width;
    int _height;
    QVector<QPointF> _inputs;

};

inline InputManager& get_input_manager()
{
    return InputManager::get_inst();
}

#endif // INPUTMANAGER_H
