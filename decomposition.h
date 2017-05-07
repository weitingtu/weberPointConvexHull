#ifndef DECOMPOSITION_H
#define DECOMPOSITION_H

#include <QObject>

class Decomposition : public QObject
{
    Q_OBJECT
public:
    static Decomposition& get_inst()
    {
        static Decomposition inst;
        return inst;
    }

signals:

public slots:
private:
    explicit Decomposition(QObject *parent = 0);
};

inline Decomposition& get_decomposition()
{
    return Decomposition::get_inst();
}

#endif // DECOMPOSITION_H
