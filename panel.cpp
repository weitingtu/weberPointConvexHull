#include "panel.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>

Panel::Panel(QWidget *parent) : QWidget(parent),
    _clear(new QPushButton(tr("&Clear"), this)),
    _input_size(new QSpinBox(this)),
    _generate(new QPushButton(tr("&Generate"), this))
{
    _input_size->setValue(20);
    _input_size->setMinimum(0);
    _input_size->setMaximum(100);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(_clear);
    vbox->addWidget(_createInputGroup());
    vbox->addStretch(1);
    setLayout(vbox);
}

QGroupBox* Panel::_createInputGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Input Setup"));

    QRadioButton *radio1 = new QRadioButton(tr("&Random"));
    QRadioButton *radio2 = new QRadioButton(tr("&Manual"));

    radio1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(_input_size);
    vbox->addWidget(_generate);
    vbox->addWidget(radio2);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

int Panel::get_input_size() const
{
    return _input_size->value();
}
