#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QLabel>
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

    void set_hex_secs(int s) const { _hex_secs->setText(QString::number(s) + " ms");}
    void set_con_secs(int s) const { _con_secs->setText(QString::number(s) + " ms");}
    void set_cdt_secs(int s) const { _cdt_secs->setText(QString::number(s) + " ms");}
    void set_dec_secs(int s) const { _dec_secs->setText(QString::number(s) + " ms");}
    void set_total_secs(int s) const { _total_secs->setText(QString::number(s) + " ms");}

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
    QLabel*      _hex_secs;
    QLabel*      _con_secs;
    QLabel*      _cdt_secs;
    QLabel*      _dec_secs;
    QLabel*      _total_secs;
};

#endif // PANEL_H
