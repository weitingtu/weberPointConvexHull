#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

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

public slots:
private:
    QGroupBox* _createInputGroup();

    QPushButton* _clear;
    QSpinBox*    _input_size;
    QPushButton* _generate;
};

#endif // PANEL_H
