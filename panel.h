#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include "enum.h"

class QPushButton;
class QSpinBox;
class QGroupBox;
class QComboBox;

class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(QWidget *parent = 0);

    QSpinBox*    get_font_size_spin_box() const { return _font_size; }
    QPushButton* get_clear_button() const { return _clear; }
    int get_input_size() const;
    QPushButton* get_generate_button() const { return _generate; }
    QPushButton* get_hex_button() const { return _hexagonal; }
    QPushButton* get_convex_hull_button() const { return _convex_hull; }
    QPushButton* get_convex_hull_auto_button() const { return _convex_hull_auto; }
    QPushButton* get_cdt_button() const { return _cdt; }
    QPushButton* get_decompose_button() const { return _decompose; }
    QPushButton* get_accomplish_button() const { return _accomplish; }
    QComboBox*   get_difference_button() const { return _difference; }

signals:
    void mode_changed(MODE m) const;

public slots:
private slots:
    void _random_input(bool checked) const;
    void _manual_input(bool checked) const;
private:
    QGroupBox* _createInputGroup();

    QSpinBox*    _font_size;
    QPushButton* _clear;
    QSpinBox*    _input_size;
    QPushButton* _generate;
    QPushButton* _hexagonal;
    QPushButton* _convex_hull;
    QPushButton* _convex_hull_auto;
    QPushButton* _cdt;
    QPushButton* _decompose;
    QPushButton* _accomplish;
    QComboBox*   _difference;
};

#endif // PANEL_H
