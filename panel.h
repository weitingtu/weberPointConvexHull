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
};

#endif // PANEL_H
