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

    void clear();
    void generate(int size);
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
