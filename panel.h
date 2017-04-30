#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

class QPushButton;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

    QPushButton* get_clear_button() const { return _clear; }

signals:

public slots:
private:

    QPushButton* _clear;
};

#endif // PANEL_H
