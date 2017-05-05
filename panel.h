#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include "enum.h"

class QPushButton;
class QSpinBox;
class QGroupBox;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

    QPushButton* get_clear_button() const { return _clear; }
    int get_input_size() const;
    QPushButton* get_generate_button() const { return _generate; }
    QPushButton* get_hex_button() const { return _hexagonal; }
    QPushButton* get_convex_hull_button() const { return _convex_hull; }

signals:
    void mode_changed(MODE m) const;

public slots:
private slots:
    void _random_input(bool checked) const;
    void _manual_input(bool checked) const;
private:
    QGroupBox* _createInputGroup();

    QPushButton* _clear;
    QSpinBox*    _input_size;
    QPushButton* _generate;
    QPushButton* _hexagonal;
    QPushButton* _convex_hull;
};

#endif // PANEL_H
